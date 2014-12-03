/* 
 * File:   JSONNumber.h
 * Author: hans
 *
 * Created on 03 December 2014, 19:45
 */

#ifndef JSONNUMBER_H
#define	JSONNUMBER_H

#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace JSON {

    template<typename Flags> class TokenFlags{
        Flags tokens[256];
    public:
        Flags(std::initializer_list<std::pair<int, Flags> > flags){
            auto begin = flags.begin();
            int prev = 0;
            while(prev < 256 && begin != flags.end()){
                int next = begin->first;
                if(next < prev || next >= 256){
                    std::ostringstream buf;
                    buf << "invalid character in mask '" << next << "'";
                    throw std::invalid_argument(buf.str());
                }
                for(int i = prev; i != next; ++i){
                    tokens[i] = static_cast<Flags>(0);
                }
                tokens[next] = begin->second;
                prev = next+1;
            }
            for(int i = prev; i != 256; ++i){
                tokens[i] = static_cast<Flags>(0);
            }
        };
        
        bool test(int codePoint, Flags mask, Flags result){
            return codePoint > 0 && codePoint < 256 && ((tokens[codePoint] & mask) == result);
        };
        
        bool test(int codePoint, Flags mask){
            return codePoint > 0 && codePoint < 256 && (tokens[codePoint] & mask);
        };
    };

}

#endif	/* JSONNUMBER_H */

