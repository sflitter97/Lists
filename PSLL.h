#ifndef PSLL_h
#define PSLL_h
#include "SLL.h"

namespace cop3530 {

template <typename L>
class PSLL : public SLL<L> {
public:
	PSLL();
	PSLL(const PSLL<L> &);
	PSLL(PSLL<L> &&) noexcept;
	~PSLL();
	PSLL<L>& operator=(const PSLL<L> &src);
	PSLL<L>& operator=(PSLL<L> &&src);
	void clearPool();

	//derived class	
	template <typename dataL>
	class PSLL_Iter {
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
		using self_type = PSLL_Iter;
		using self_reference = PSLL_Iter&;

		//Constructors
		explicit PSLL_Iter(cop3530::node<L>* start = nullptr ) : here( start ) {};
		PSLL_Iter( const PSLL_Iter<dataL>& src ) : here( src.here ) {};

		//Functions
		reference operator*() const {
			if (!here)
				throw std::runtime_error("Error: Past the end of the list");
			return here->data; };

		pointer operator->() const {		//need to test
			if (!here)
				throw std::runtime_error("Error: Past the end of the list");
			return ( &(operator*()) );	};

		self_reference operator=( PSLL_Iter const& src ){
			this->here = src.here;
			return *this;	};

		self_reference operator++(){
			if (!here)
				return *this;
			here = here->next;
			return *this; };	//preincrement

		self_type operator++(int) {
			if (!here)
				return *this;
			PSLL_Iter<dataL> temp(this->here);
			this->here = this->here->next;
			return temp;	}; // postincrement

		bool operator==(PSLL_Iter const& rhs) const {
			return rhs.here == this->here;	};

		bool operator!=(PSLL_Iter const& rhs) const {
			return rhs.here != this->here; 	};
	};
	
	using size_t = std::size_t;
	using value_type = L;
	using iterator = PSLL_Iter<L>;
	using const_iterator = PSLL_Iter<L const>;
	
	iterator begin() {return PSLL_Iter<L>(this->head);}
	iterator end() {return PSLL_Iter<L>(nullptr);}
	const_iterator begin() const {return PSLL_Iter<L const>(this->head);}
	const_iterator end() const {return PSLL_Iter<L const>(nullptr);}

private:
	int freelist_length = 0;
	int usedlist_length = 0;
	typename cop3530::node<L>* freeList_head = nullptr;
	typename cop3530::node<L>* temp;
	typename cop3530::node<L>* add_node() override;				//Helper Method
	void delete_node(typename cop3530::node<L>* erased) override;	//Helper Method
	int freeList_size();
};

template <typename L>
PSLL<L>::PSLL() {
	freeList_head = nullptr;
}

template <typename L>
PSLL<L>::PSLL(const PSLL<L> &src) {
	L* temp = src.contents();
	freeList_head = nullptr;
	usedlist_length = src.usedlist_length;
	size_t size = src.length();
	for (size_t i = 0; i < size; i++)
		this->push_back(temp[i]);
	delete [] temp;
}

template <typename L>
PSLL<L>::PSLL(PSLL<L> &&src) noexcept {
	this->head = src.head;		src.head = nullptr;
	this->tail = src.tail;		src.tail = nullptr;
	freeList_head = src.freeList_head;		src.freeList_head = nullptr;
	freelist_length = src.freelist_length;	src.freelist_length = 0;
	usedlist_length = src.usedlist_length;	src.usedlist_length = 0;
}

template <typename L>
PSLL<L>::~PSLL() {
	this->clear();
	clearPool();
}

template <typename L>
PSLL<L>& PSLL<L>::operator=(const PSLL<L> &src) {
	if (this != & src) {
		this->clear();
		this->clearPool();
		L* temp = src.contents();
		size_t size = src.length();
		for (size_t i = 0; i < size; i++)
			this->push_back(temp[i]);
		delete [] temp;
		freeList_head = src.freeList_head;
		freelist_length = src.freelist_length;
		usedlist_length = src.usedlist_length;
	}
	return *this;
}

template <typename L>
PSLL<L>& PSLL<L>::operator=(PSLL<L> &&src) {
	if (this != &src) {
		this->head = src.head;		src.head = nullptr;
		this->tail = src.tail;		src.tail = nullptr;
		freeList_head = src.freeList_head;		src.freeList_head = nullptr;
		freelist_length = src.freelist_length;	src.freelist_length = 0;
		usedlist_length = src.usedlist_length;	src.usedlist_length = 0;
	}
	return *this;
}

template <typename L>
void PSLL<L>::delete_node(cop3530::node<L>* erased) {
	if (freelist_length >= 50) {
		delete erased;
		return;
	}
	usedlist_length--;
	freelist_length++;
	if (freeList_head) {
		temp = freeList_head;
		freeList_head = erased;
		freeList_head->next = temp;
	}
	else {
		freeList_head = erased;
		freeList_head->next = nullptr;
	}

	//Manages memory by cutting down the pool if it's to large
	if (usedlist_length >= 100) {
		int number_of_deletions =  freelist_length - (usedlist_length / 2);
		for (int i = 0; i < number_of_deletions; i++) {
			temp = freeList_head;
			freeList_head = freeList_head->next;
			delete temp;
		}
	}
}

template <typename L>
cop3530::node<L>* PSLL<L>::add_node() {
	if (!freeList_head)
		temp = new cop3530::node<L>;
	else {
		temp = freeList_head;
		freeList_head = freeList_head->next;
		freelist_length--;
		temp->next = nullptr;
	}
	usedlist_length++;
	return temp;
}

template <typename L>
int PSLL<L>::freeList_size() {
	int counter = 0;
	temp = freeList_head;
	while (temp) {
		temp = temp->next;
		counter++;
	}
	return counter;
}

template <typename L>
void PSLL<L>::clearPool() {
	temp = freeList_head;
	while (freeList_head) {
		freeList_head = freeList_head->next;
		delete temp;
		temp = freeList_head;
	}
}

}
#endif //PSLL.h