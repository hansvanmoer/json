
/*
#include <iostream>

#include <sstream>

#include "Test.h"
#include "JSON.h"

using namespace Test;

using namespace JSON;

using Reader = JSONReader<char>;
using Node = typename Reader::Node;

void testRootNumber(){
	Reader reader{std::istringstream{"  10"}};
	Node node = reader.root();
	assertEquals("test integer as root node ",10, node.value<int>());
}

void testRootBoolean(){

}

void testRootString(){

}

void testRootObject(){

}

void test(){
	testRootNumber();
};

TEST_MAIN(test)*/

int main(int count, const char **args){
    return 0;
}