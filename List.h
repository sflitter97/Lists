#ifndef _LIST_H
#define _LIST_H
#include <iostream>
#include <string>
#include <stdexcept>

namespace cop3530 {

template <class L>
class List {
	public:
		virtual ~List() {};
		virtual void insert(const L& element, size_t position) = 0;		//finished
		virtual void push_back(const L& element) = 0;					//finished
		virtual void push_front(const L& element) = 0;					//finished
		virtual L replace(const L& element, size_t position) = 0;		//finished
		virtual L remove(size_t position) = 0;					//Catch tested
		virtual L pop_back() = 0;								//Catch tested
		virtual L pop_front() = 0;								//Catch tested
		virtual const L& item_at(size_t position) const = 0;				//MAKE THE ELEMENT CONST.  NEED CONST VERSIONS OF THIS FUNCTION
		virtual const L& peek_back() const = 0;						//MAKE THE ELEMENT CONST
		virtual const L& peek_front() const = 0;						//MAKE THE ELEMENT CONST
		virtual L& item_at(size_t position) = 0;				//MAKE THE ELEMENT CONST.  NEED CONST VERSIONS OF THIS FUNCTION
		virtual L& peek_back() = 0;						//MAKE THE ELEMENT CONST
		virtual L& peek_front()= 0;						//MAKE THE ELEMENT CONST
		virtual bool is_empty() const = 0;						//Catch tested
		virtual bool is_full() const = 0;						//Catch tested
		virtual size_t length() const = 0;						//Catch tested
		virtual void clear() = 0;								//Catch tested
		virtual bool contains(const L& element, bool (*equals_function)(const L&, const L&)) const = 0;			///finished
		virtual std::ostream& print(std::ostream& output) const = 0;	//Catch tested
		virtual L* contents() const = 0;								//Catch tested
};
}
#endif //LIST_h