/* 
 * File:   JSONFlags.h
 * Author: hans
 *
 * Created on 03 December 2014, 20:11
 */

#ifndef JSONFLAGS_H
#define	JSONFLAGS_H

#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace JSON{
    
    template<typename Flags> class TokenFlags{
        Flags tokens_[256];
    public:
        TokenFlags(std::initializer_list<std::pair<int, Flags> > flags){
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
                    tokens_[i] = static_cast<Flags>(0);
                }
                tokens_[next] = begin->second;
                prev = next+1;
                ++begin;
            }
            for(int i = prev; i != 256; ++i){
                tokens_[i] = static_cast<Flags>(0);
            }
        };
        
        bool test(int codePoint, Flags mask, Flags result) const{
            return codePoint >= 0 && codePoint < 256 && ((tokens_[codePoint] & mask) == result);
        };
        
        bool test(int codePoint, Flags mask) const{
            return codePoint >= 0 && codePoint < 256 && (tokens_[codePoint] & mask);
        };
        
        bool operator()(int codePoint, Flags mask, Flags result) const{
            return test(codePoint, mask, result);
        };
        
        bool operator()(int codePoint, Flags mask) const{
            return test(codePoint, mask);
        };
    };

}

#endif	/* JSONFLAGS_H */

