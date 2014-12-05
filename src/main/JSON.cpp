#include "JSON.h"

#include "Log.h"

#include <cctype>

using namespace JSON;

const unsigned int WHITESPACE_MASK = 0x00000001;
const unsigned int NUMBER_MASK = 0x00000002;
const unsigned int STRING_ESCAPE_MASK = 0x00000004;
const unsigned int INVALID_STRING_MASK = 0x00000008;
const unsigned int STRING_UNESCAPE_MASK = 0x00000010;
const unsigned int STRING_ESCAPE_TOKEN_MASK = 0x00FF0000;
const unsigned int STRING_UNESCAPE_TOKEN_MASK = 0xFF000000;

const unsigned int STRING_ESCAPE_TOKEN_SHIFT = 16;
const unsigned int STRING_UNESCAPE_TOKEN_SHIFT = 24;

#define JSON_2_TOKENS(number) (number), (number)

#define JSON_4_TOKENS(number) JSON_2_TOKENS((number)), JSON_2_TOKENS((number))

#define JSON_8_TOKENS(number) JSON_4_TOKENS((number)), JSON_4_TOKENS((number))

#define JSON_16_TOKENS(number) JSON_8_TOKENS((number)), JSON_8_TOKENS((number))

#define JSON_32_TOKENS(number) JSON_16_TOKENS((number)), JSON_16_TOKENS((number))

#define JSON_64_TOKENS(number) JSON_32_TOKENS((number)), JSON_32_TOKENS((number))

#define JSON_ESCAPE_TOKEN(number) (STRING_ESCAPE_MASK | ((number) << STRING_ESCAPE_TOKEN_SHIFT))

#define JSON_UNESCAPE_TOKEN(number) (STRING_UNESCAPE_MASK | ((number) << STRING_UNESCAPE_TOKEN_SHIFT))

#define JSON_ESCAPE_UNESCAPE_TOKEN(number) JSON_ESCAPE_TOKEN((number)) | JSON_UNESCAPE_TOKEN((number))

/*
 * quick and dirty lookup table
 * 
 * 1111 1111 0001 
 * 
 */
const unsigned int tokens [128] = {
    JSON_8_TOKENS(INVALID_STRING_MASK), //0x00 - 0x007
    INVALID_STRING_MASK | JSON_ESCAPE_TOKEN(0x62), //0x08 backspace
    INVALID_STRING_MASK | WHITESPACE_MASK | JSON_ESCAPE_TOKEN(0x74), //0x09 tab
    INVALID_STRING_MASK | WHITESPACE_MASK | JSON_ESCAPE_TOKEN(0x6E), //0x0A line feed
    INVALID_STRING_MASK, //0x0B vertical tab
    INVALID_STRING_MASK | JSON_ESCAPE_TOKEN(0x66), //0x0C form feed
    INVALID_STRING_MASK | WHITESPACE_MASK | JSON_ESCAPE_TOKEN(0x72), //0x0D carriage return
    JSON_16_TOKENS(INVALID_STRING_MASK), //0x0E - 0x1D
    JSON_2_TOKENS(INVALID_STRING_MASK), //0x1E - 0x1F
    WHITESPACE_MASK, //0x20 space
    0, //0x21 !
    INVALID_STRING_MASK | JSON_ESCAPE_UNESCAPE_TOKEN(0x22), //0x22 quotation mark
    JSON_8_TOKENS(0), //0x23 - 0x2A,
    NUMBER_MASK, //0x2B +
    0, //0x2C ,
    JSON_2_TOKENS(NUMBER_MASK), //0x2D - 0x2E -, .
    JSON_ESCAPE_UNESCAPE_TOKEN(0x2F), //0x2F solidus
    JSON_8_TOKENS(NUMBER_MASK), JSON_2_TOKENS(NUMBER_MASK), //0x30 - 0x39 numbers
    JSON_8_TOKENS(0),0,0,0,                //0x3A - 0x44
    NUMBER_MASK,                           //0x45 E
    JSON_16_TOKENS(0), JSON_4_TOKENS(0),0,0, //0x46 - 0x5B
    INVALID_STRING_MASK | JSON_ESCAPE_UNESCAPE_TOKEN(0x5C), //0x5C reverse solidus
    JSON_4_TOKENS(0),                                       //0x5D - 0x60
    0, //0x61 a
    JSON_UNESCAPE_TOKEN(0x08), //0x62 b
    0, 0, //0x63 - 0x64 c,d
    NUMBER_MASK, //0x65 e
    JSON_UNESCAPE_TOKEN(0x0C), //0x66 f
    0, 0, 0, 0, 0, 0, 0, //0x67 - 0x6D g - m
    JSON_UNESCAPE_TOKEN(0x0A), //0x6E n
    0, 0, 0, //0x6F - 0x71 o - q
    JSON_UNESCAPE_TOKEN(0x0D), //0x72 r
    0, //0x73 s
    JSON_UNESCAPE_TOKEN(0x09), //0x74 t
    JSON_8_TOKENS(0), JSON_2_TOKENS(0), //0x75 - 0x7E u - ~
    INVALID_STRING_MASK //0x7F delete
};


const int JSON::Tokens::OBJECT_BEGIN = '{';

const int JSON::Tokens::OBJECT_END = '}';

const int JSON::Tokens::ARRAY_BEGIN = '[';

const int JSON::Tokens::ARRAY_END = ']';

const int JSON::Tokens::ELEMENT_SEPARATOR = ',';

const int JSON::Tokens::KEY_VALUE_SEPARATOR = '=';

const int JSON::Tokens::STRING_DELIMITER = '\"';

const int JSON::Tokens::ESCAPE = '\\';

const int JSON::Tokens::UNICODE_ESCAPE = 'u';

const int JSON::Tokens::CARRIAGE_RETURN = '\r';

const int JSON::Tokens::FORM_FEED = '\f';

const int JSON::Tokens::LINE_FEED = '\n';

const int JSON::Tokens::QUOTE = '\"';

const int JSON::Tokens::SOLIDUS = '/';

const int JSON::Tokens::BACKSPACE = '\b';

const int JSON::Tokens::TAB = '\t';

const int JSON::Tokens::REVERSE_SOLIDUS = '\\';

const std::string JSON::Tokens::LITERAL_TRUE{"true"};

const std::string JSON::Tokens::LITERAL_FALSE{"false"};

const std::string JSON::Tokens::LITERAL_NULL{"null"};

bool JSON::Tokens::whitespace(int code) {
    return code > 0 && code < 0x80 && ((tokens[code] & WHITESPACE_MASK) != 0);
}

int JSON::Tokens::unescape(int code) {
    if (code > 0 && code < 128) {
        return static_cast<int>((tokens[code] & STRING_UNESCAPE_TOKEN_MASK) >> STRING_UNESCAPE_TOKEN_SHIFT);
    } else {
        return 0;
    }
}

int JSON::Tokens::escape(int code) {
    if (code > 0 && code < 128) {
        return static_cast<int>((tokens[code] & STRING_ESCAPE_TOKEN_MASK) >> STRING_ESCAPE_TOKEN_SHIFT);
    } else {
        return 0;
    }
};

bool JSON::Tokens::number(int code) {
    return code >= 0 && code < 128 && ((tokens[code] & NUMBER_MASK) != 0);
};

bool JSON::Tokens::invalidInString(int code){
    return code >= 0 && code < 128 && ((tokens[code] & INVALID_STRING_MASK) != 0);
};

std::ostream &JSON::operator<<(std::ostream &output, const NodeType &type) {
    switch (type) {
        case NodeType::ARRAY:
            output << "array";
            break;
        case NodeType::BOOLEAN:
            output << "boolean";
            break;
        case NodeType::NULL_VALUE:
            output << "null";
            break;
        case NodeType::NUMBER:
            output << "number";
            break;
        case NodeType::OBJECT:
            output << "object";
            break;
        case NodeType::STRING:
            output << "string";
            break;
    }
    return output;
}
