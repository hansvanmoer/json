#include "JSON.h"

#include "../Test.h"

#include <set>
#include <sstream>
#include <map>

using namespace JSON::Tokens;

using namespace std;

void testWhitespace(){
    
    std::set<int> whitespace{ {
        0x20, 0x09, 0x0A, 0x0D
    } };
    
    for(int i = 0; i < 10000; ++i){
        std::ostringstream buffer;
        buffer << "test for whitespace failed: " << std::ios::hex << i;
        Test::assertTrue(buffer.str(), (whitespace.find(i) != whitespace.end()) == JSON::Tokens::whitespace(i));
    }
}

void testNumber(){
    std::set<int> numbers{ {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', 'e', 'E', '.'
    } };
      
    for(int i = 0; i < 10000; ++i){
        bool expected = (numbers.find(i) != numbers.end());
        bool real = JSON::Tokens::number(i);
        if(expected != real){
            Test::fail("number test failed for codepoint '", i, "' expected ", expected, " but was ",real);
        }
    }
}

void testEscape(){
    std::map<int,int> escapes = {
        {'\"', '\"'}, 
        {'\\','\\'}, 
        {'/', '/'}, 
        {'\b', 'b'},
        {'\f', 'f'},
        {'\n', 'n'},
        {'\r', 'r'},
        {'\t', 't'},
    };
    
    for(int i = 1; i < 10000; ++i){
        int real = JSON::Tokens::escape(i);
        auto found = escapes.find(i);
        int expected = found == escapes.end() ? 0 : found->second;
        if(expected != real){
            std::ostringstream buffer;
            Test::fail("escape test failed for codepoint '", i, "' expected ", expected, " but was ",real);
        }
    }
}

void testUnescape(){
    std::map<int,int> unescapes = {
        {'\"', '\"'}, 
        {'\\','\\'}, 
        {'/', '/'}, 
        {'b', '\b'},
        {'f', '\f'},
        {'n', '\n'},
        {'r', '\r'},
        {'t', '\t'},
    };
    
    for(int i = 1; i < 10000; ++i){
        int real = JSON::Tokens::unescape(i);
        auto found = unescapes.find(i);
        int expected = found == unescapes.end() ? 0 : found->second;
        if(expected != real){
            std::ostringstream buffer;
            Test::fail("escape test failed for codepoint '", i, "' expected ", expected, " but was ",real);
        }
    }
}

void test(){
    testWhitespace();
    testNumber();
    testEscape();
    testUnescape();
}

TEST_MAIN("tokens",test);

