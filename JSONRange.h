#ifndef JSON_RANGE_H
#define JSON_RANGE_H

#include <iterator>

namespace JSON{

	template<typename Iterator> class ForwardRange{
		private:
			Iterator begin_;
			Iterator end_;
		public:	
			using IteratorType = Iterator;
			using ValueType = typename std::iterator_traits<Iterator>::value_type;

			ForwardRange(Iterator begin, Iterator end) : begin_(begin), end_(end){};
			
			explicit ForwardRange(Iterator begin) : begin_(begin), end_(){};
			
			Iterator begin() const{
				return begin_;
			};

			Iterator end() const{
				return end_;
			};

			bool valid() const{
				return begin_ != end_;
			};

			operator bool() const{
				return begin_ != end_;
			};

			bool operator!() const{
				return begin_ == end_;
			};

			ForwardRange<Iterator> &operator++(){
				begin_++;
				return *this;
			};

			ForwardRange<Iterator> operator++(int){
				ForwardRange<Iterator> range{*this};
				begin_++;
				return range;
			};
			
			bool operator==(const ForwardRange<Iterator> &range) const{
				return begin_ == range.begin_;
			};

			bool operator!=(const ForwardRange<Iterator> &range) const{
				return begin_ != range.begin_;
			};

			ValueType operator*() const{
				return *begin_;
			};
	};
}

namespace std{

	template<typename Iterator> class iterator_traits<Core::ForwardRange<Iterator> >{
		public:
			using difference_type = Core::ForwardRange<typename iterator_traits<Iterator>::difference_type>; //Can't do this
			using value_type = typename iterator_traits<Iterator>::value_type;
			using pointer = typename iterator_traits<Iterator>::pointer;
			using reference = typename iterator_traits<Iterator>::reference;
			using iterator_category = std::forward_iterator_tag;
	};
}

#endif
