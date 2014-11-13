#include "../Test.h"
#include "JSONTypePolicy.h"
#include "JSONTraits.h"
#include "TypePolicyTest.h"

using namespace JSON;
using namespace std;

void test(){
    BasicTypePolicy<BasicTraits> policy;
    testTypePolicy(policy);
    JSON::BasicTypePolicy<BasicTraits, std::string, MyLiteral<double>, MyLiteral<bool> > policy2;
    testTypePolicy(policy2);
}

TEST_MAIN("basic type policy",test);