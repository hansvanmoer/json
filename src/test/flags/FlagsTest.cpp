#include "../Test.h"

#include "JSONFlags.h"

using namespace JSON;

void testNoFlags() {
    Log::log("testing unset flags");
    JSON::TokenFlags<unsigned int> flags{};
    for (int i = -2; i < 270; ++i) {
        if (flags(i, 0x20) || flags(i, 0x20, 0x20)) {
            Test::fail("flag was set for codepoint ", i, ", expected not set");
        }
    }
}

void testNegativeFlag() {
    Log::log("testing negative flags");
    try {
        JSON::TokenFlags<unsigned int> flags{
            {
                {-1, 0x10},
                {1, 0x20}
            }
        };
        Test::fail("negative flag accepted, expected invalid argument exception");
    } catch (std::invalid_argument &e) {
        //good!
    }
}

void testWrongFlagOrder() {
    Log::log("testing invalid flag order");
    try {
        JSON::TokenFlags<unsigned int> flags{
            {
                {2, 0x10},
                {5, 0x20},
                {4, 0x30}
            }
        };
        Test::fail("invalid flag order accepted, expected invalid argument exception");
    } catch (std::invalid_argument &e) {
        //good!
    }
}

void testNotSet(TokenFlags<unsigned int> &flags, int start, int end){
    for(int i = start; i < end; ++i){
        for(unsigned int mask = 0x01; mask <= 0x80; mask<<=1){
            if (flags(i, mask) || flags(i, mask, mask)) {
                Test::fail("flag was set for codepoint ", i, ", expected not set");
            }
        }
    }
}

void testSet(TokenFlags<unsigned int> &flags, int codePoint, unsigned int mask){
    if (!flags(codePoint, mask) || !flags(codePoint, mask, mask)) {
        Test::fail("flag was not set for codepoint ", codePoint, ", expected set");
    }
}

void testFlags(std::initializer_list<std::pair<int, unsigned int> > args){
    JSON::TokenFlags<unsigned int> flags{args};
    auto begin = args.begin();
    int prev = -10;
    while(begin != args.end()){
        testNotSet(flags, prev, begin->first);
        testSet(flags, begin->first, begin->second);
        prev = begin->first+1;
        ++begin;
    }
    testNotSet(flags, prev, 270);
};

void testFlags() {
    Log::log("checking flags");
    testFlags(
        {
            {1, 0x10},{2,0x20},{4, 0x50}
        }
    );
    Log::log("checking first and last flags");
    testFlags(
        {
            {0, 0x10},{2,0x20},{255, 0x50}
        }
    );
}

void test() {
    testNoFlags();
    testNegativeFlag();
    testWrongFlagOrder();
    testFlags();
}

TEST_MAIN("flags test", test);