/* 
 * File:   TypePolicyTest.h
 * Author: hans
 *
 * Created on 24 October 2014, 19:58
 */

#ifndef TYPEPOLICYTEST_H
#define	TYPEPOLICYTEST_H

#include <string> 

template<typename T> class MyLiteral{
private:
    T value_;
public:
    MyLiteral(T value) : value_(value){}; 
    
    bool operator==(const MyLiteral &d) const{
        return value_ == d.value_;
    };
    
    T value() const{
        return value_;
    };
};

template<typename T> std::ostream &operator<<(std::ostream &output, const MyLiteral<T> &literal){
    output << literal.value();
    return output;
};

template<typename Policy> void testTypePolicy(Policy &policy){
    Test::assertEquals("number cast failed", typename Policy::Number{3.0}, policy.toNumber(3.0));
    Test::assertEquals("boolean cast failed", typename Policy::Boolean(true), policy.toBoolean(true));
    Test::assertEquals("boolean cast failed", typename Policy::Boolean(false), policy.toBoolean(false));
    
    std::string str{"abc"};
    
    Test::assertEquals("string cast failed", str, policy.toString(str));
}

#endif	/* TYPEPOLICYTEST_H */

