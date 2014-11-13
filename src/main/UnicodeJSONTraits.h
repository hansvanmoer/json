#ifndef JSON_JSON_UNICODE_TRAITS_H
#define JSON_JSON_UNICODE_TRAITS_H

namespace JSON{

	template<typename Char_, typename CharTraits_, typename Allocator_> class UnicodeJSONTraits{
		public:
			using Char = Char_;
			using CharTraits = CharTraits_;
			using Allocator = Allocator_;

			using InputStream = std::basic_istream<Char, CharTraits>;
			using OutputStream = std::basic_ostream<Char, CharTraits>;

			using String = std::basic_string<Char, CharTraits, Allocator>;

		private:


			static const Char LEFT_CURLY_BRACKET	{0x7B};

			static const Char RIGHT_CURLY_BRACKET	{0x7D};

			static const Char LEFT_SQUARE_BRACKET	{0x5B};

			static const Char RIGHT_SQUARE_BRACKET	{0x5D};

			static const Char COLON			{0x3A};

			static const Char COMMA			{0x2C};

			static const Char QUOTE			{0x22};

			static const Char SOLIDUS		{0x2F};

			static const Char REVERSE_SOLIDUS	{0x5C};

			static const Char LINE_FEED		{0x0A};

			static const Char BACKSPACE		{0x08};

			static const Char FORM_FEED		{0x0C};



			static const Char CARRIAGE_RETURN	{0x0D};

			static const Char CHARACTER_TABULATION	{0x09};


		public:
			static bool whitespace(Char c) const noexcept;

			static bool newLine(Char c) const noexcept{
			
			};

			static constexpr Char newLine() const noexcept{
				return LINE_FEED;
			};

			static bool objectBegin(Char c) noexcept{
				return CharTraits::eq(LEFT_CURLY_BRACKET,c);
			};

			static constexpr Char objectBegin() noexcept;

			static bool objectEnd(Char c) noexcept;

			static constexpr Char objectEnd() noexcept;



