#include "JSONStoragePolicy.h"

using namespace JSON;

NodeTypeException::NodeTypeException(NodeType expected, NodeType real) : JSONException("wrong node type"), expected_(expected), real_(real){}

NodeType NodeTypeException::expectedType() const{
    return expected_;
}

NodeType NodeTypeException::nodeType() const{
    return real_;
}



