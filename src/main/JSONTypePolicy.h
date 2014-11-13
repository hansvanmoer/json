/* 
 * File:   JSONTypePolicy.h
 * Author: hans
 *
 * Created on 24 October 2014, 19:25
 */

#ifndef JSONTYPEPOLICY_H
#define	JSONTYPEPOLICY_H

#include <string>

namespace JSON {

    template<
    typename JSONTraits,
    typename String_ = std::string,
    typename Number_ = double,
    typename Boolean_ = bool
    > class BasicTypePolicy {
    public:
        using Number = Number_;
        using Boolean = Boolean_;
        using String = String_;

        static Number toNumber(typename JSONTraits::Number value) {
            return static_cast<Number> (value);
        };

        static Boolean toBoolean(typename JSONTraits::Boolean value) {
            return static_cast<Boolean> (value);
        };

        static String toString(typename JSONTraits::String value) {
            return value;
        };
    };

}

#endif	/* JSONTYPEPOLICY_H */

