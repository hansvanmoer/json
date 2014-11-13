#include "JSON.h"

#include "Log.h"

#include <cctype>

using namespace JSON;

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

bool *createWhitespaceTable() {
    bool *table = new bool[0x21];
    for (int i = 0; i < 0x21; ++i) {
        table[i] = false;
    }
    table[0x09] = true;
    table[0x0A] = true;
    table[0x0D] = true;
    table[0x20] = true;
    return table;
}

bool JSON::Tokens::whitespace(int code) {
    static const bool *table = createWhitespaceTable();
    return code > 0 && code < 0x21 && table[code];
}

int *createUnescapeTable() {
    int *table = new int[128];
    for (int i = 0; i < 128; ++i) {
        table[i] = 0;
    }
    table['\"'] = '\"';
    table['\\'] = '\\';
    table['/'] = '/';
    table['b'] = '\b';
    table['f'] = '\f';
    table['n'] = '\n';
    table['r'] = '\r';
    table['t'] = '\t';
    return table;
}

int JSON::Tokens::unescape(int code) {
    static const int *table = createUnescapeTable();
    if (code > 0 && code < 128) {
        return table[code];
    } else {
        return 0;
    }
}

int *createEscapeTable() {
    int *table = new int[128];
    for (int i = 0; i < 128; ++i) {
        table[i] = 0;
    }
    table['\"'] = '\"';
    table['\\'] = '\\';
    table['/'] = '/';
    table['\b'] = 'b';
    table['\f'] = 'f';
    table['\n'] = 'n';
    table['\r'] = 'r';
    table['\t'] = 't';
    return table;
}

int JSON::Tokens::escape(int code) {
    static const int *table = createEscapeTable();
    if (code > 0 && code < 128) {
        return table[code];
    } else {
        return 0;
    }
};

bool *createNumberTable() {
    bool *table = new bool[101];
    for (int i = 0; i < 101; ++i) {
        table[i] = 0;
    }
    table['0'] = true;
    table['1'] = true;
    table['2'] = true;
    table['3'] = true;
    table['4'] = true;
    table['5'] = true;
    table['6'] = true;
    table['7'] = true;
    table['8'] = true;
    table['9'] = true;
    table['.'] = true;
    table['-'] = true;
    table['+'] = true;
    table['e'] = true;
    table['E'] = true;
    return table;
}

bool JSON::Tokens::number(int code) {
    static bool *table = createNumberTable();
    return code > 0 && code < 102 && table[code];
};

std::ostream &JSON::operator<<(std::ostream &output, const NodeType &type){
    switch(type){
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
