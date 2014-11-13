/* 
 * File:   StoragePolicyTraits.h
 * Author: hans
 *
 * Created on 25 October 2014, 11:21
 */

#ifndef STORAGEPOLICYTRAITS_H
#define	STORAGEPOLICYTRAITS_H

namespace Test{

    struct Traits{

        using NodeId = unsigned long;
        using String = std::string;
        using Number = double;
        using Boolean = bool;
    };

}

#endif	/* STORAGEPOLICYTRAITS_H */

