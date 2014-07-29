#ifndef IO_JSON_H
#define IO_JSON_H

#include "JSONRange.h"
#include "JSONParser.h"

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <vector>
#include <stack>

namespace JSON{

	class JSONException : public std::runtime_error{
		public:
			JSONException(std::string msg);
	};

	class JSONSyntaxException : public JSONException{
		public:
			JSONSyntaxException(std::string msg, int line, int column);

			int line() const;

			int column() const;
		private:
			int line_;
			int column_;
	};

	template<typename Char, typename CharTraits> class JSONFieldNotFoundException : public JSONException{
		public:
			using FieldName = std::basic_string<Char,CharTraits>;
		private:
			FieldName fieldName_;
		public:
			JSONFieldNotFoundException(FieldName fieldName) : JSONException(std::string{"field not found"}), fieldName_(fieldName){};
			
			FieldName fieldName() const{
				return fieldName_;
			};
	};

	template<typename Char_, typename CharTraits_ = std::char_traits<Char_>, typename Allocator_ = std::allocator<Char_> > class UnicodeJSONTraits{
		public:

			using Char = Char_;
			using CharTraits = CharTraits_;
			using Allocator = Allocator_;
			using String = std::basic_string<Char, CharTraits>;
		public:

			constexpr static Char stringDelimiter() noexcept{
				return '\"';
			};

			static bool stringDelimiter(const Char c) noexcept{
				return CharTraits::eq(c,'\"');
			};

			constexpr static Char stringEscape() noexcept{
				return '\\';
			};

			static bool stringEscape(const Char c) noexcept{
				return CharTraits::eq(c,'\\');
			};

			static bool stringEscaped(const char c) noexcept{
				return CharTraits::eq(c,'\"');
			};

			constexpr static Char objectBegin() noexcept{
				return '{';
			};

			static bool objectBegin(const Char c) noexcept{
				return CharTraits::eq(c,'{');
			};

			constexpr static Char objectEnd() noexcept{
				return '}';
			};

			static bool objectEnd(const Char c) noexcept{
				return CharTraits::eq(c,'}');
			};

			constexpr static Char arrayBegin() noexcept{
				return '[';
			};

			static bool arrayBegin(const Char c) noexcept{
				return CharTraits::eq(c,'[');
			};

			constexpr static Char arrayElementSeparator() noexcept{
				return ',';
			};

			static bool arrayElementSeparator(const Char c) noexcept{
				return CharTraits::eq(c,',');
			};

			constexpr static Char arrayEnd() noexcept{
				return ']';
			};

			static bool arrayEnd(const Char c) noexcept{
				return CharTraits::eq(c,']');
			};

			constexpr static Char keyValueSeparator() noexcept{
				return ':';
			};

			static bool keyValueSeparator(const Char c) noexcept{
				return CharTraits::eq(c,':');
			};

			constexpr static Char indentation() noexcept{
				return static_cast<Char>(0x09);
			};

			//todo: optimize
			static bool whitespace(const Char c) noexcept{
				return CharTraits::eq(c,' ') || 
					CharTraits::eq(c,static_cast<Char>(0x09)) || 
					CharTraits::eq(c, static_cast<Char>(0x0A)) || 
					CharTraits::eq(c,static_cast<Char>(0x0D));
			};

			constexpr static Char whitespace() noexcept{
				return ' ';
			}; 

			constexpr static Char lineEnd() noexcept{
				return '\n';
			};

			static bool lineEnd(const Char c) noexcept{
				return CharTraits::eq(c,'\n');
			};

			constexpr static String booleanTrue(){
				return std::string{std::initializer_list<Char>{'t','r','u','e'}};
			};

			constexpr static String booleanFalse(){
				return std::string{std::initializer_list<Char>{'f','a','l','s','e'}};
			};

			constexpr static String nill(){
				return std::string{std::initializer_list<Char>{'n','u','l','l'}};
			};

			static String toString(const char *input){
				std::basic_stringstream<Char,CharTraits> buffer;
				while(*input){
					buffer.put(*input);
					++input;
				}
				return buffer.str();
			};

	};

	enum class NodeType{
		VALUE_TYPE, ARRAY_TYPE, OBJECT_TYPE
	};

	template<typename Char_ = char, typename JSONTraits_ = UnicodeJSONTraits<Char_> > class JSONReader{
		public:
			using Char = Char_;
			using JSONTraits = JSONTraits_;
			using CharTraits = typename JSONTraits::CharTraits;
			using Allocator = typename JSONTraits::Allocator;
			using InputStream = std::basic_istream<Char,CharTraits>;
			using OutputStream = std::basic_ostream<Char,CharTraits>;
			using String = std::basic_string<Char, CharTraits, Allocator>;
			using FieldNotFoundException = JSONFieldNotFoundException<Char,CharTraits>;

		private:

			using Reader = JSONReader<Char_, JSONTraits_>;

			using Buffer = ForwardRange<Char *>;
			using ReadBuffer = ForwardRange<const Char *>;
	
			struct BufferedString{
				private:
					ReadBuffer buffer_;
					std::size_t length_;
				public:
					BufferedString() : buffer_(), length_(0){};

					BufferedString(const Char *begin, std::size_t length) : buffer_(begin, begin+length), length_(length){};

					BufferedString(const Char *begin, const Char *end) : buffer_(begin, end), length_(static_cast<std::size_t>(end-begin)){};
			
					ReadBuffer buffer() const{
						return buffer_;
					};

					bool operator<(BufferedString &string) const{
						if(length_ == string.length_){
							return length_ < string.length_;
						}else{
							Buffer current{buffer_};
							Buffer second{string.buffer_};
							while(current){
								if(CharTraits::lt(*current,*second)){
									return true;
								}
								current++;
								second++;
							}
							return false;
						}
					};
			};

			struct Key{
				const Key *parent;
				BufferedString id;
				NodeType type;

				Key() : parent(), id(), type(NodeType::VALUE_TYPE){};

				Key(const Key *parent_) : parent(parent_), id(), type(NodeType::VALUE_TYPE){};

				Key(const Key *parent_, BufferedString id_) : parent(parent_), id(id), type(NodeType::VALUE_TYPE){};

				Key(const Key *parent_, const BufferedString &id_, NodeType type_) : parent(parent_), id(id_), type(type_){};
			};

			struct KeyComp{

				bool operator()(const Key &first, const Key &second) const{
					if(first.parent == second.parent){
						return first.id < second.id;				
					}else{
						return first.parent < second.parent;
					}
				};
			};
	
			using NodeMap = std::map<const Key *, ReadBuffer, KeyComp>;

			using Entry = std::pair<const Key *, ReadBuffer>;

			using OutputStringStream = std::basic_ostringstream<Char, CharTraits>;
			using InputStringStream = std::basic_istringstream<Char,CharTraits>;
	
		public:
			using NodeId = const Key *;
			using FieldId = String;

			class Node{
				private:
				Entry entry_;

				public:
					Node() : entry_(){};
					
					Node(const Entry &entry) : entry_(entry){};
					
					NodeId id() const{
						return entry_->first;
					};

					NodeId parentId() const{
						return entry_->first->parent;
					};

					NodeType type(){
						return entry_->first->type;
					};

					template<typename T> T value() const{
						InputStringStream buffer{std::string(entry_->second.begin(), entry_->second.end())};
						T value;
						buffer >> value;
						return value;
					};

					void writeValue(OutputStream &output) const{
						output.write(entry_->second.begin(), static_cast<std::size_t>(entry_->second.end() - entry_->second.begin()));
					};

					operator bool() const{
						return entry_;
					};

					bool operator!() const{
						return !entry_;
					};

			};

		private:
			InputStream &input_;

			Char *buffer_;
			std::size_t bufferSize_;

			NodeMap nodes_;
			Entry entry_;

			void fillBuffer(){
				std::streampos begin = input_.tellg();
				input_.seekg(0, std::ios::end);
				bufferSize_ = static_cast<std::size_t>(input_.tellg() - begin);
				input_.seekg(0, std::ios::beg);
				if(bufferSize_ != 0 && input_.good()){
					buffer_ = new Char[bufferSize_];
					input_.read(buffer_, bufferSize_);
				}else{
					throw IO::IOException(std::string{"could not read input stream"});
				}
			};

			void dispose(const Key *key = nullptr){
				delete key;
				for(auto i = nodes_.begin(); i != nodes_.end(); i++){
					delete i->first;
				}
				nodes_.clear();
				delete[] buffer_;
			};

			void raise(const char *message, Buffer buffer, const Key *key = nullptr){
				int line=0;
				int column=0;
				Buffer cursor{buffer_, buffer.end()};
				while(cursor){
					skip(cursor, [&](const Char c){
						if(JSONTraits::lineEnd(c)){
							line++;
							column=0;
						}else{
							column++;
						}
					});
				}
				dispose(key);
				throw JSONSyntaxException(std::string{message}, line, column);
			};

			void skipWhitespace(Buffer &buffer){
				buffer = IO::skip(buffer, JSONTraits::whitespace);
			};

			void check(const Buffer &buffer, const Key *key = nullptr){
				if(!buffer){
					raise("unexpected end of stream",buffer, key);
				}
			};

			void skipWhitespaceAndCheck(Buffer &buffer, const Key *key = nullptr){
				skipWhitespace(buffer);
				check(buffer, key);
			};

			ReadBuffer unescapeString(Buffer &buffer, const Key *key = nullptr){
				Char *begin = buffer.begin();
				Char *dest = begin;
				Char c = *buffer;
				while(!JSONTraits::stringDelimiter(c)){
					if(JSONTraits::stringEscape(c)){
						++buffer;
						c=*buffer;
					}
					*dest = c;
					++buffer;
					++dest;
					check(buffer, key);
					c = *buffer;
				}
				return ReadBuffer{begin, dest};
			};

			ReadBuffer parseValue(Buffer &buffer, const Key *key = nullptr){
				Char *begin = buffer.begin();
				buffer = skip(buffer, [](Char c){return !JSONTraits::keyValueSeparator(c) && !JSONTraits::arrayElementSeparator(c) && !JSONTraits::whitespace(c);});
				check(buffer, key);
				return ReadBuffer{begin, buffer.begin()};
			};

			const Key *parseArray(const Key *parent, const BufferedString &id, Buffer &buffer);

			const Key *parseObject(const Key *parent, const BufferedString &id, Buffer &buffer);

			const Key *parseValue(const Key *parent, const BufferedString &id, Buffer &buffer){
				Char c = *buffer;
				const Key *key;
				if(JSONTraits::stringDelimiter(c)){
					++buffer;
					key = new Key{parent, id, NodeType::VALUE_TYPE};
					nodes_.push(std::make_pair(key, unescapeString(buffer, key)));
				}else if(JSONTraits::arrayBegin(c)){
					++buffer;
					key = parseArray(parent, id, buffer);
				}else if(JSONTraits::objectBegin(c)){
					++buffer;
					key = parseObject(parent, id, buffer);
				}else{
					key = new Key{parent, id, NodeType::VALUE_TYPE};
					nodes_.push(std::make_pair(key, parseValue(buffer, key)));
				}
				return key;
			};

			const Key *parseField(const Key *parent, Buffer &buffer){
				if(JSONTraits::stringDelimiter(*buffer)){
					++buffer;
					ReadBuffer idBuffer = unescapeString(buffer);
					skipWhitespaceAndCheck(buffer);
					if(JSONTraits::keyValueSeparator(*buffer)){
						++buffer;
						skipWhitespaceAndCheck(buffer);
						return parseValue(parent, idBuffer, buffer);
					}
				}
				raise("unexpected character",buffer);
			};

			void parse(){
				Buffer buffer{buffer_, buffer_+bufferSize_};
				skipWhitespaceAndCheck(buffer);
				parseValue(0,BufferedString{},buffer);
				skipWhitespace(buffer);
				if(!buffer){
					raise("unexpected character", buffer);
				}
			};

			JSONReader(const JSONReader<Char_,JSONTraits_> &) = delete;
			JSONReader<Char_, JSONTraits_> &operator=(const JSONReader<Char_, JSONTraits_> &) = delete;

		public:
			JSONReader(InputStream &input) : input_(input), buffer_(), bufferSize_(), nodes_(){
				Reader::fill();
				Reader::parse();
			};

			JSONReader(JSONReader<Char_, JSONTraits_> &&reader) : input_(reader.input_), buffer_(), bufferSize_(0), nodes_(){
				std::swap(buffer_, reader.buffer_);
				reader.bufferSize_ = 0;
				std::swap(nodes_, reader.nodes_);
			};

			~JSONReader(){
				dispose();
			};

			Node find(NodeId id){
				Entry entry = nodes_.find(id);
				if(entry != nodes_.end()){
					return Node{entry};
				}else{
					return Node{};
				}
			};

			Node findChild(NodeId parentId, FieldId fieldId){
				Key key{parentId,BufferedString{fieldId.data(), fieldId.length()}};
				Entry entry = nodes_.find(&key);
				if(entry != nodes_.end()){
					return Node{entry};
				}else{
					return Node{};
				}
			};

			Node findChild(NodeId parentId){
				Key key{parentId, BufferedString{}};
				Entry entry = nodes_.find(&key);
				if(entry != nodes_.end()){
					return Node{entry};
				}else{
					return Node{};
				}		
			};

			Node get(NodeId id){
				Entry entry = nodes_.find(id);
				if(entry != nodes_.end()){
					return Node{entry};
				}else{
					throw JSONException("node not found");
				}			
			};

			Node getChild(NodeId parentId, FieldId fieldId){
				Key key{parentId,BufferedString{fieldId.data(), fieldId.length()}};
				Entry entry = nodes_.find(&key);
				if(entry != nodes_.end()){
					return Node{entry};
				}else{
					throw JSONException("node not found");
				}		
			};

			Node getChild(NodeId parentId){
				Key key{parentId, BufferedString{}};
				Entry entry = nodes_.find(&key);
				if(entry != nodes_.end()){
					return Node{entry};
				}else{
					throw JSONException("node not found");
				}		
		
			};
	};

	template<typename Char_, typename JSONTraits_ = UnicodeJSONTraits<Char_> > class JSONWriter{
		public:
			using Char = Char_;
			using JSONTraits = JSONTraits_;
			using CharTraits = typename JSONTraits::CharTraits;
			using Allocator = typename JSONTraits::Allocator;
			using OutputStream = std::basic_ostream<Char, CharTraits>;
			using String = std::basic_string<Char, CharTraits, Allocator>;
		private:
			
			OutputStream &output_;
			bool first_;
			std::vector<Char> indent_;
			std::stack<NodeType> scopes_;

			JSONWriter<Char_,JSONTraits_> &operator=(const JSONWriter<Char_,JSONTraits_> &) = delete;
			
			JSONWriter(const JSONWriter<Char_,JSONTraits_> &) = delete;

			template<typename End> void endLine(End c){
				output_ << c << JSONTraits::lineEnd();
				endLine();
			};

			template<typename End> void endLineBefore(End c){
				output_ << JSONTraits::lineEnd();
				endLine();
				output_ << c;
			};

			void endLine(){
				output_.write(indent_.data(), indent_.size());
			};

			void doBeginObject(){
				scopes_.push(NodeType::OBJECT_TYPE);
				first_=true;
				indent_.push_back(JSONTraits::indentation());
				endLine(JSONTraits::objectBegin());
			};

			void doEndObject(){
				if(scopes_.top() != NodeType::OBJECT_TYPE){
					throw JSONException(std::string{"element mismatch: object end"});
				}
				scopes_.pop();
				indent_.pop_back();
				first_=false;
				endLineBefore(JSONTraits::objectEnd());
			};
	
			void doBeginArray(){
				scopes_.push(NodeType::ARRAY_TYPE);
				first_=true;
				indent_.push_back(JSONTraits::indent());
				endLine(JSONTraits::arrayBegin());
			};

			void doEndArray(){
				if(scopes_.top() != NodeType::ARRAY_TYPE){
					throw JSONException(std::string{"element mismatch: array end"});
				}
				scopes_.pop();
				indent_.pop_back();
				first_=false;
				endLineBefore(JSONTraits::arrayEnd());
			};

			void escape(const String &value){
				output_ << JSONTraits::stringDelimiter();
				const Char *begin = value.data();
				std::size_t len{0};
				for(auto i = value.begin(); i != value.end(); ++i){
					if(JSONTraits::stringEscape(*i)){
						output_.write(begin, len);
						++i;
						if(i != value.end()){
							output_ << JSONTraits::stringEscape() << *i;
							begin+=(len+1);
							len=0;
						}else{
							throw JSONException("unclosed escape");
						}
					}else{
						++len;
					}
				}
				output_.write(begin, len);
				output_ << JSONTraits::stringDelimiter();
			};

			void doWriteId(const String &id){
				if(!first_){
					endLine(JSONTraits::arrayElementSeparator());
				}
				escape(id);
				output_ << JSONTraits::whitespace() << JSONTraits::keyValueSeparator() << JSONTraits::whitespace();
			};

			void canWriteUnnamed(){
				if(scopes_.empty()){
					if(!first_){
						throw JSONException("unnamed element not allowed here");
					}
				}else if(scopes_.top() == NodeType::ARRAY_TYPE){
					throw JSONException("unnamed element not allowed here");
				}
			};

			void canWriteNamed(){
				if(scopes_.empty() || scopes_.top() != NodeType::OBJECT_TYPE){
					throw JSONException("named element not allowed here");
				}
			};

			void doWriteSeparator(){
				if(!first_){
					endLine(JSONTraits::arrayElementSeparator());
				}
			};

			template<typename T> void doWriteValue(T value){
				output_ << value;
				first_=false;
			};

			void doWriteValue(const String &value){
				escape(value);
				first_=false;
			};

		public:
			JSONWriter(OutputStream &output) : output_(output), first_(true){};

			JSONWriter(OutputStream &&output) : output_(output), first_(true){};
			
			~JSONWriter(){};
			
			void beginObject(){
				canWriteUnnamed();
				doBeginObject();
			};

			void beginObject(std::string id){
				canWriteNamed();
				doWriteId(id);
				doBeginObject();
			};

			void beginObject(const Char *id){
				beginObject(String{id});
			};

			void endObject(){
				if(scopes_.empty() || scopes_.top() != NodeType::OBJECT_TYPE){
					throw JSONException("object closure mismatched");
				}
				doEndObject();
			};

			void beginArray(){
				canWriteUnnamed();
				doBeginArray();
			};

			void beginArray(std::string id){
				canWriteNamed();
				doWriteId(id);
				doBeginArray();
			};

			void beginArray(const Char *id){
				beginArray(String{id});
			};

			void endArray(){
				if(scopes_.empty() || scopes_.top() != NodeType::ARRAY_TYPE){
					throw JSONException("object closure mismatched");
				}
				doEndArray();
			};

			void write(long value){
				canWriteUnnamed();
				doWriteSeparator();
				doWriteValue(value);
			};

			void write(String id, long value){
				canWriteNamed();
				doWriteSeparator();
				doWriteId(id);
				doWriteValue(value);
			};

			void write(const char *id, long value){
				write(String{id}, value);
			};

			void write(unsigned long value){
				canWriteUnnamed();
				doWriteSeparator();
				doWriteValue(value);
			};

			void write(String id, unsigned long value){
				canWriteNamed();
				doWriteSeparator();
				doWriteId(id);
				doWriteValue(value);
			};

			void write(const char *id, unsigned long value){
				write(String{id}, value);
			};

			void write(double value){
				canWriteUnnamed();
				doWriteSeparator();
				doWriteValue(value);
			};

			void write(String id, double value){
				canWriteNamed();
				doWriteSeparator();
				doWriteId(id);
				doWriteValue(value);
			};

			void write(const char *id, double value){
				write(String{id}, value);
			};

			void write(bool value){
				canWriteUnnamed();
				doWriteSeparator();
				doWriteValue(value);
			};

			void write(String id, bool value){
				canWriteNamed();
				doWriteSeparator();
				doWriteId(id);
				doWriteValue(value);
			};

			void write(const char *id, bool value){
				write(String{id}, value);
			};

			void write(String value){
				canWriteUnnamed();
				doWriteSeparator();
				doWriteValue(value);
			};

			void write(String id, String value){
				canWriteNamed();
				doWriteSeparator();
				doWriteId(id);
				doWriteValue(value);
			};

			void write(const char *id, String value){
				String id_{id};
				canWriteNamed();
				doWriteSeparator();
				doWriteId(id);
				doWriteValue(value);
			};

	};
}

template<typename Char_, typename JSONTraits_> auto JSON::JSONReader<Char_,JSONTraits_>::parseArray(
		const Key *parent, 
		const BufferedString &id, 
		Buffer &buffer) -> const Key * {
	const Key *key = new Key{parent, id, NodeType::ARRAY_TYPE};
	const Key *root = key;
	nodes_.push(std::make_pair(key, ReadBuffer{}));
	while(buffer){
		skipWhitespaceAndCheck(buffer);
		key = parseValue(key, BufferedString{}, buffer);
		if(JSONTraits::arrayElementSeparator(*buffer)){
			++buffer;
			skipWhitespaceAndCheck(buffer);
		}else if(JSONTraits::arrayEnd(*buffer)){
			++buffer;
			skipWhitespace(buffer);
			return root;
		}else{
			raise("unexpected character", buffer);
		}
	}
	raise("unexpected end of stream", buffer);
};

template<typename Char_, typename JSONTraits_> auto JSON::JSONReader<Char_,JSONTraits_>::parseObject(
		const Key *parent, 
		const BufferedString &id, 
		Buffer &buffer) -> const Key *{
	const Key *key = new Key{parent, id, NodeType::OBJECT_TYPE};
	nodes_.push(std::make_pair(key, ReadBuffer{}));
	while(buffer){
		skipWhitespaceAndCheck(buffer);
		parseField(key, buffer);
		if(JSONTraits::fieldSeparator(*buffer)){
			++buffer;
		}else{
			raise("unexpected character", buffer);
		}
	}
	raise("unexpected end of stream", buffer);
};

#endif
