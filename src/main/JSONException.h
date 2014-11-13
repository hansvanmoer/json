#ifndef JSON_EXCEPION_H
#define JSON_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace JSON{

	class JSONException : public std::runtime_error{
        public:
		JSONException(std::string msg);
	
	};
}

#endif
