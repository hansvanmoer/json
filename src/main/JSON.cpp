#include "JSON.h"

using namespace JSON;

JSONException::JSONException(std::string msg) : std::runtime_error(msg){}

JSONSyntaxException::JSONSyntaxException(std::string msg, int line, int column) : JSONException(msg), line_(line), column_(column){}

int JSONSyntaxException::line() const{
	return line_;
}

int JSONSyntaxException::column() const{
	return column_;
}
