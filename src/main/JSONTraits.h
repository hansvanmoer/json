/* 
 * File:   JSONTraits.h
 * Author: hans
 *
 * Created on 31 October 2014, 14:28
 */

#ifndef JSONTRAITS_H
#define	JSONTRAITS_H

#include <string>

namespace JSON {

    class BasicTraits {
    public:
        using String = std::string;
        using Number = double;
        using Boolean = bool;
    };



}

#endif	/* JSONTRAITS_H */

