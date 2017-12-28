#ifndef SSLL_H
#define SSLL_H
#include "SLL.h"
#include "node.h"

namespace cop3530 {

template <typename L>
class SSLL : public SLL<L> {
public:
	SSLL();
	SSLL(const SSLL<L> &src);				//fixed
	SSLL(SSLL<L> &&) noexcept;
	~SSLL();
	SSLL<L>& operator=(const SSLL<L> &);	//fixed
	SSLL<L>& operator=(SSLL<L> &&);

	//derived class	
	template <typename dataL>
	class SSLL_Iter {
	private:
		cop3530::node<L>* here;

	public:	
		//The type aliases required for c++ iterator compatibility
		using value_type = dataL;
		using reference = dataL&;
		using pointer = dataL*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		
		//These are used to clarify the meaning of the code
		using self_type = SSLL_Iter;
		using self_reference = SSLL_Iter&;

		//Constructors
		explicit SSLL_Iter(cop3530::node<L>* start = nullptr ) : here( start ) {};				//TESTED
		SSLL_Iter( const SSLL_Iter<dataL>& src ) : here( src.here ) {};

		//Functions
		reference operator*() const {		//TESTED
			if (!here)
				throw std::runtime_error("Error: Past the end of the list");
			return here->data; };

		pointer operator->() const {		//TESTED
			if (!here)
				throw std::runtime_error("Error: Past the end of the list");
			return ( &(operator*()) );	};

		self_reference operator=( SSLL_Iter const& src ){				//TESTED
			this->here = src.here;
			return *this;	};

		self_reference operator++(){					//TESTED
			if (!here)
				return *this;
			here = here->next;
			return *this; };	//preincrement

		self_type operator++(int) {						//TESTED
			if (!here)
				return *this;
			SSLL_Iter<dataL> temp(this->here);
			this->here = this->here->next;
			return temp;	}; // postincrement

		bool operator==( SSLL_Iter<dataL> const& rhs ) const {				//TESTED
			return rhs.here == this->here;	};

		bool operator!=( SSLL_Iter<dataL> const& rhs) const {				//TESTED
			return rhs.here != this->here; 	};
	};
	
	using size_t = std::size_t;
	using value_type = L;
	using iterator = SSLL_Iter<L>;
	using const_iterator = SSLL_Iter<L const>;
	
	iterator begin() {return SSLL_Iter<L>(this->head);}
	iterator end() {return SSLL_Iter<L>(nullptr);}
	const_iterator begin() const {return SSLL_Iter<L const>(this->head);}
	const_iterator end() const {return SSLL_Iter<L const>(nullptr);}


private:
	cop3530::node<L>* add_node() override;					//Helper Method
	void delete_node(cop3530::node<L>* erased) override;		//Helper Method
};

template <typename L>
SSLL<L>::SSLL() {}

template <typename L>
SSLL<L>::SSLL(const SSLL<L> &src) {
	L* temp = src.contents();
	for (int i = (src.length() - 1); i >= 0; i--)
		this->push_front(temp[i]);
	delete [] temp;
}

template <typename L>
SSLL<L>::SSLL(SSLL<L> &&src) noexcept {
	this->head = src.head;
	this->tail = src.tail;
	src.head = nullptr;
	src.tail = nullptr;
}

template <typename L>
SSLL<L>& SSLL<L>::operator=(const SSLL<L> &src) {
	if (this != &src) {
		this->clear();
		L* temp = src.contents();
		size_t size = src.length();
		for (size_t i = 0; i < size; i++)
			this->push_back(temp[i]);
		delete [] temp;
	}
	return *this;
}

template <typename L>
SSLL<L>& SSLL<L>::operator=(SSLL<L> &&src) {
	if (this != &src) {
		this->clear();
		this->head = src.head;
		this->tail = src.tail;
		src.head = nullptr;
		src.tail = nullptr;
	}
	return *this;
}

template <typename L>
SSLL<L>::~SSLL() {
	this->clear();
}

template <typename L>
void SSLL<L>::delete_node(cop3530::node<L>* erased) {
	delete erased;
}

template <typename L>
cop3530::node<L>* SSLL<L>::add_node() {
	cop3530::node<L>* temp = new cop3530::node<L>;
	return temp;
}

}
#endif //SSLL.h