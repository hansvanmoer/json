#ifndef JSON_PARSER_H
#define JSON_PARSER_H

namespace JSON{

	template<typename Range, typename Predicate> Range skip(Range range, Predicate predicate){
		while(range && predicate(*range)){
			++range;
		}
		return range;
	};
}

#endif
