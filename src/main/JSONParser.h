#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "JSON.h"

#include <string>
#include <iterator>

namespace JSON {

    template<typename JSONTraits, typename Range, typename Builder> class Parser {
    private:

        using Char = typename JSONTraits::Char;
        using CharTraits = typename JSONTraits::CharTraits;
        
        class ParseException {
        };
        
        struct Buffer{
            Range begin;
            Range end;
            
            Buffer() : begin(), end(){};
            Buffer(Range begin_) : begin(begin_), end(){};
            Buffer(Range begin_, Range end_) : begin(begin_), end(end_){};
        };
        
        Range input_;
        Builder &builder_;

        Buffer id_;
        bool named_;
        const char *errorMessage_;

        Parser(const Parser<JSONTraits, Range, Builder> &) = delete;

        Parser<JSONTraits, Range, Builder> &operator=(const Parser<JSONTraits, Range, Builder> &) = delete;
       
        void raise(const char *msg){
            errorMessage_ = msg;
            throw ParseException{};
        };
        
        Buffer parseString(){
            Buffer value(input_);
            while(input_){
                Char c = *input_;
                if(CharTraits::eq(c, Tokens::STRING_DELIMITER)){
                    value.end = input_;
                    return value;
                }else if(CharTraits::eq(c, Tokens::ESCAPE)){
                    ++input_;
                    if(input_){
                        c = *input_;
                        if(CharTraits::eq(c, Tokens::UNICODE_ESCAPE)){
                            c = parseUnicodeCharacter();
                        }else{
                            c = 
                        }
                    }else{
                        raise("unexpected end of input: expected escape code");
                    }
                }
            }
            raise("unexpected end of input: expected string ending token");
        };
        
        void addString(Buffer value){
            if(named_){
                builder_.string(id.begin(), id.end(), value.begin(), value.end());
            }else{
                builder_.string(value.begin(), value.end());
            }
        };
        
        void addNull(){
            if(named_){
                builder_.null(id.begin(), id.end());
            }else{
                builder_.null();
            }
        };
        
        void addBoolean(bool value){
            if(named_){
                builder_.boolean(id.begin(), id.end(), value);
            }else{
                builder_.boolean(value);
            }
        };
        
        void addNumber(double value){
            if(named_){
                builder_.number(id.begin(), id.end(), value);
            }else{
                listener.number(value);
            }
        };
        
        void beginObject(){
            if(named_){
                builder_.beginObject(id.begin(), id.end());
            }else{
                builder_.beginObject();
            }
        };
        
        void endObject(){
            builder_.endObject();
        };
        
        void beginArray(){
            if(named_){
                builder_.beginArray(id.begin(), id.end());
            }else{
                builder_.beginArray();
            }
        };
        
        void endArray(){
            builder_.endArray();
        };
        
    public:

        Parser(Range input, EventListener &listener) : input_(input), builder_(listener), id_(), named_(false), errorMessage_() {
        };
        
        void parse();

    };

    template<typename Iterator_, typename DocumentBuilder_> class BasicJSONParser {
    public:
        using Iterator = Iterator_;
        using CharacterRange = Range<Iterator>;
        using Character = typename CharacterRange::Character;
        using DocumentBuilder = DocumentBuilder_;
    private:

        Iterator begin_;
        CharacterRange current_;
        DocumentBuilder &builder;

        std::ostringstream buffer_;

        class Raise {
        private:
            const char *message_;
        public:

            Raise(const char *message) : message_(message) {
            };

            const char *message() const {
                return message_;
            };
        };
    public:

        BasicJSONParser(DocumentBuilder &builder, Iterator begin, Iterator end) : begin_(begin), current_(begin, end), builder_(builder), buffer_(), nodes_() {
        };

        void addString(const std::string &id) {
            if (id.empty()) {
                builder_.addString(id, buffer_.str());
            } else {
                builder_.addString(buffer_.str());
            }
        };

        void addBoolean(bool literal) {
            std::string id{buffer_.str()};
            if (id.empty()) {
                builder_.addBoolean(literal);
            } else {
                builder_.addBoolean(buffer.str(), literal);
            }
        };

        void addNull() {
            std::string id{buffer_.str()};
            if (id.empty()) {
                builder_.addNull();
            } else {
                builder_.addNull(id);
            }
        };

        void beginObject() {
            std::string id{buffer_.str()};
            if (id.empty()) {
                builder_.beginObject();
            } else {
                builder_.beginObject(id);
            }
        };

        void beginArray() {
            std::string id{buffer_.str()};
            if (id.empty()) {
                builder_.beginArray();
            } else {
                builder_.beginArray(id);
            }
        };

        void endObject() {
            builder_.endObject();
        };

        void endArray() {
            builder_.endArray();
        };

        template<typename Predicate> void skipWhile(Predicate pred) {
            while (current_ && pred(*current_)) {
                ++current_;
            }
        };

        template<typename Predicate> bool skip(Predicate pred) {
            if (current_) {
                if (pred(*current)) {
                    ++current_;
                    return true;
                } else {
                    return false;
                }
            } else {
                raise("unexpected end of document");
            }
        };

        bool skipLiteral(int code) {
            if (current_) {
                if (*current_ == code) {
                    ++current_;
                    return true;
                } else {
                    return false;
                }
            } else {
                raise("unexpected end of document");
            }
        };

        template<typename LiteralIterator> bool skipLiteral(LiteralIterator begin, LiteralIterator end) {
            CharacterRange range{current_};
            while (current_ && *begin == *current_) {
                ++current_;
                ++begin;
                if (begin == end) {
                    return true;
                }
            }
            current_ = range;
            return false;
        };

        void clearBuffer() {
            buffer_.str("");
            buffer_.clear();
        };

        void raise(const char *message) {
            throw Raise(message);
        };

        void parseBranch();

        void parseString() {
            clearBuffer();
            while (current_) {
                int code = *current_;
                if (code == Tokens::ESCAPE) {
                    ++current_;
                    if (current_) {
                        code = *current_;
                        if (code == Tokens::UNICODE_ESCAPE) {
                            buffer_.put(parseUnicode());
                        } else {
                            code = unescape(code);
                            if (code == 0) {
                                raise("invalid escape character");
                            } else {
                                buffer_.put(code);
                            }
                        }
                    } else {
                        raise("unexpected end of document: unclosed escape literal");
                    }
                } else if (code == Tokens::STRING_DELIMITER) {
                    ++current_;
                    return;
                } else {
                    buffer_.put(code);
                }
                ++current_;
            }
            raise("unexpected end of document: unclosed string literal");
        };

        void parseNumber() {
            std::string id{buffer_.str()};
            clearBuffer();
            while (current_ && Tokens::number(*current_)) {
                buffer_.put(*current_);
                ++current_;
            }
            double value;
            buffer_ >> value;
            if (buffer_.bad()) {
                raise("invalid literal");
            }
            if (id.empty()) {
                builder_.addNumber(id, value);
            } else {
                builder_.addNumber(value);
            }
        };

        void parseLiteral() {
            if (skipLiteral(Tokens::LITERAL_TRUE.begin(), Tokens::LITERAL_TRUE.end())) {
                addBoolean(true);
            } else if (skipLiteral(Tokens::LITERAL_FALSE.begin(), Tokens::LITERAL_FALSE.end())) {
                addBoolean(false);
            } else if (skipLiteral(Tokens::LITERAL_NULL.begin(), Tokens::LITERAL_NULL.end())) {
                addNull();
            } else {
                parseNumber();
            }
        };

        void parseObject() {
            while (true) {
                skipWhile(Tokens::whitespace);
                parseString();
                beginObject();
                skip(Tokens::whitespace);
                if (skipLiteral(Tokens::KEY_VALUE_SEPARATOR)) {
                    skipWhile(Tokens::whitespace);
                    parseBranch();
                    skipWhile(Tokens::whitespace);
                    if (skipLiteral(Tokens::OBJECT_END)) {
                        endObject();
                        skipWhile(Tokens::whitespace);
                        return;
                    } else if (!skipLiteral(Tokens::ELEMENT_SEPARATOR)) {
                        raise("unexpected token: expected element separator or object end");
                    }
                } else {
                    raise("unexpected token: expected assignment");
                }
            }
        };

        void parseArray() {
            beginArray();
            while (true) {
                skipWhile(Tokens::whitespace);
                parseBranch();
                skipWhile(Tokens::whitespace);
                if (skipLiteral(Tokens::ARRAY_END)) {
                    endArray();
                    skipWhile(Tokens::whitespace);
                    return;
                } else if (!skipLiteral(Tokens::ELEMENT_SEPARATOR)) {
                    raise("unexpected token: expected element separator or array end");
                }
            }
        };

        void parse() {
            try {
                parseBranch();
            } catch (Raise &raise) {
                Iterator end = current_.begin();
                int line = 0;
                int column = 0;
                for (Iterator i = begin_; i != end; ++i) {
                    if (Tokens::newLine(*i)) {
                        ++line;
                    } else {
                        ++column;
                    }
                }
                builder_.error(raise.message(), line, column);
            }
        };
    };
}

template<typename Iterator_, typename DocumentBuilder_> void JSON::BasicJSONParser::parseBranch() {
    if (skip(Tokens::OBJECT_BEGIN)) {
        parseObject();
    } else if (skip(Tokens::ARRAY_BEGIN)) {
        parseArray();
    } else if (skip(Tokens::STRING_DELIMITER)) {
        std::string id{buffer_.str()};
        parseString();
        addString(id);
    } else {
        parseLiteral();
    }
};

#endif
