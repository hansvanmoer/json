#include "JSONStoragePolicy.h"

#include "StoragePolicyTest.h"

#include "Test.h"

using namespace JSON;

using namespace std;

using Policy = StrictStoragePolicy<Test::Traits>;
using NodeData = typename Policy::NodeData;


void testException(NodeTypeException &e, NodeType nodeType, NodeType expectedType){
    Test::assertEquals("wrong type exception node type", nodeType, e.nodeType());
    Test::assertEquals("wrong type exception expected type", expectedType, e.expectedType());
}

void testWrongType(Policy &policy, NodeData data, NodeType type){
    if(type != NodeType::ARRAY){
        try{
            policy.arrayElements(data);
            Test::fail("expected invalid type exception when accessing array elements in node of type ", type);
        }catch(NodeTypeException &e){
            testException(e, type, NodeType::ARRAY);
        }
    }
    if(type != NodeType::OBJECT){
        try{
            policy.objectId(data);
            Test::fail("expected invalid type exception when accessing object id in node of type ", type);
        }catch(NodeTypeException &e){
            testException(e, type, NodeType::OBJECT);
        }
    }
    if(type != NodeType::NULL_VALUE){
        try{
            policy.null(data);
            Test::fail("expected invalid type exception when accessing null value in node of type ", type);
        }catch(NodeTypeException &e){
            testException(e, type, NodeType::NULL_VALUE);
        }
    }
    if(type != NodeType::BOOLEAN){
        try{
            policy.boolean(data);
            Test::fail("expected invalid type exception when accessing boolean in node of type ", type);
        }catch(NodeTypeException &e){
            testException(e, type, NodeType::BOOLEAN);
        }
    }
    if(type != NodeType::NUMBER){
        try{
            policy.number(data);
            Test::fail("expected invalid type exception when accessing number in node of type ", type);
        }catch(NodeTypeException &e){
            testException(e, type, NodeType::NUMBER);
        }
    }
    if(type != NodeType::STRING){
        try{
            policy.string(data);
            Test::fail("expected invalid type exception when accessing string in node of type ", type);
        }catch(NodeTypeException &e){
            testException(e, type,NodeType::STRING);
        }
    }
}

void testArray(){
    Log::log("test array storage");
    Policy policy;
    NodeData data = policy.createArray();
    std::list<NodeData> &elements = policy.arrayElements(data);
    elements.push_back(data);
    elements = policy.arrayElements(data);
    Test::assertEquals("wrong element array size", 1, elements.size());
    Test::assertEquals("wrong element array content", data, *elements.begin());
    testWrongType(policy, data, NodeType::ARRAY);
    policy.destroyNode(data);
}

void testObject(){
    Log::log("test object storage");
    Test::Traits::NodeId id = 1L;
    Policy policy;
    NodeData data = policy.createObject(id);
    Test::assertEquals("wrong object id", id, policy.objectId(data));
    testWrongType(policy, data, NodeType::OBJECT);
    policy.destroyNode(data);
}


void testNull(){
    Log::log("test null storage");
    Policy policy;
    NodeData data = policy.createNull();
    policy.null(data);
    testWrongType(policy, data, NodeType::NULL_VALUE);
    policy.destroyNode(data);
}


void testString(){
    Log::log("test string storage");
    Test::Traits::String str = "test";
    Policy policy;
    NodeData data = policy.createString(str);
    Test::assertEquals("wrong object id", str, policy.string(data));
    testWrongType(policy, data, NodeType::STRING);
    policy.destroyNode(data);
}


void testNumber(){
    Log::log("test number storage");
    Test::Traits::Number number = 1.0;
    Policy policy;
    NodeData data = policy.createNumber(number);
    Test::assertEquals("wrong number", number, policy.number(data));
    testWrongType(policy, data, NodeType::NUMBER);
    policy.destroyNode(data);
}


void testBoolean(){
    Log::log("test boolean storage");
    Test::Traits::Boolean b = true;
    Policy policy;
    NodeData data = policy.createBoolean(b);
    Test::assertEquals("wrong number", b, policy.boolean(data));
    testWrongType(policy, data, NodeType::BOOLEAN);
    policy.destroyNode(data);
}


void test(){
    testArray();
    testObject();
    testNull();
    testString();
    testNumber();
    testBoolean();
}

TEST_MAIN("strict storage policy",test);