#ifndef JSON_JSON_H
#define JSON_JSON_H

#include <string>
#include <iostream>

namespace JSON {
    
    namespace Tokens {

        extern const int OBJECT_BEGIN;

        extern const int OBJECT_END;

        extern const int ARRAY_BEGIN;

        extern const int ARRAY_END;

        extern const int ELEMENT_SEPARATOR;

        extern const int KEY_VALUE_SEPARATOR;

        extern const int STRING_DELIMITER;

        extern const int ESCAPE;

        extern const int UNICODE_ESCAPE;

        extern const int CARRIAGE_RETURN;

        extern const int FORM_FEED;

        extern const int LINE_FEED;

        extern const int QUOTE;

        extern const int SOLIDUS;

        extern const int BACKSPACE;

        extern const int TAB;

        extern const int REVERSE_SOLIDUS;

        extern const std::string LITERAL_TRUE;

        extern const std::string LITERAL_FALSE;

        extern const std::string LITERAL_NULL;

        bool whitespace(int code);

        int unescape(int code);

        int escape(int code);

        bool number(int code);
    }

    enum class NodeType{
        OBJECT, ARRAY, NUMBER, STRING, BOOLEAN, NULL_VALUE
    };
    
    std::ostream &operator<<(std::ostream &output, const JSON::NodeType &type);
}



#endif
