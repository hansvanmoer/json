/* 
 * File:   JSONNumber.h
 * Author: hans
 *
 * Created on 03 December 2014, 19:45
 */

#ifndef JSONNUMBER_H
#define	JSONNUMBER_H

#include <sstream>
#include <stdexcept>
#include <string>

namespace JSON {

    template<typename Char, typename CharTraits> class NumberParser {
    private:
        
    public:
        NumberParser(){};
        
        template<typename Range> double parse(Range range){
            std::ostringstream buffer;
            while(range){
                typename CharTraits::int_type value = CharTraits::to_int_type(*range);
                if(Traits::number(value)){
                    buffer.put(value);
                }else{
                    try{
                        return std::stold(buffer.str());
                    }catch(invalid_argument &e){
                        
                    }catch(out_of_range &e){
                        
                    }
                }
                ++range;
            }
            return parse();
        };

    };

}

#endif	/* JSONNUMBER_H */

