#include "../Test.h"

#include "JSONInputBuffer.h"

using namespace Test;
using namespace std;
using namespace JSON;

void test(){
    
    std::string data{"this is a test"};
    
    std::istringstream input{data};
    LazyInputBuffer<char, std::char_traits<char> > buffer(input,3);
    
    LazyInputBuffer<char, char_traits<char> >::Range range = buffer.read();
    
    std::string::iterator i = data.begin();
    while(i!=data.end() && range){
        assertEquals("invalid buffer data",*i,*range);
        ++i;
        ++range;
    }
    assertTrue("buffer too short", i == data.end());
    assertTrue("buffer too long", !range);
    
}

TEST_MAIN("lazy input buffer", test);