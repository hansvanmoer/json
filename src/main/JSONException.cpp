#include "JSONException.h"

using namespace JSON;

JSONException::JSONException(std::string msg) : std::runtime_error(msg){};
