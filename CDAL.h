#ifndef CDAL_H
#define CDAL_H
#include "List.h"
#include "c_node.h"

namespace cop3530 {

template <typename L>
class CDAL : public List<L> {
	public:
		CDAL();			//FINISHED
		CDAL(const CDAL<L> &);
		CDAL(CDAL<L> &&) noexcept;
		~CDAL();		//FINISHED
		CDAL<L>& operator=(const CDAL<L> &);
		CDAL<L>& operator=(CDAL<L> &&);
		void insert(const L& element, size_t position) override;		//FINISHED
		void push_back(const L& element) override;						//FINISHED
		void push_front(const L& element) override;					//FINISHED
		L replace(const L& element, size_t position) override;			//FINISHED
		L remove(size_t position) override;						//FINISHED			
		L pop_back() override;									//FINISHED
		L pop_front() override;									//FINISHED
		L& item_at(size_t position) override;					//FINISHED
		L& peek_back() override;							//FINISHED
		L& peek_front() override;							//FINISHED
		const L& item_at(size_t position) const override;
		const L& peek_back() const override;
		const L& peek_front() const override;	
		bool is_empty() const override;							//FINISHED
		bool is_full() const override;							//FINISHED
		size_t length() const override;							//FINISHED
		void clear() override;									//FINISHED
		bool contains(const L& element, bool (*equals_function)(const L&, const L&)) const override;
		std::ostream& print(std::ostream& output) const override;		//FINISHED
		L* contents() const override;									//FINISHED

	private:
/*		struct node	{
			L data;
			node* next = nullptr;
			node* previous = nullptr;
			L* array = new L[50];
		};*/
		cop3530::c_node<L>* head;
		cop3530::c_node<L>* tail;
		cop3530::c_node<L>* curr;
		size_t full_slots;			//Shows the first open index in the array
		size_t total_slots;
		void up_size();
		void down_size();

	public:
		//derived class	
		template <typename dataL>
		class CDAL_Iter {
		private:
			cop3530::c_node<L>* here;
			size_t index;
			size_t end_index;
			size_t used_slots = 0;

		public:	
			//The type aliases required for c++ iterator compatibility
			using value_type = dataL;
			using reference = dataL&;
			using pointer = dataL*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			
			//These are used to clarify the meaning of the code
			using self_type = CDAL_Iter;
			using self_reference = CDAL_Iter&;

			//Constructors
			explicit CDAL_Iter(typename cop3530::c_node<L>* input = nullptr, size_t start = 0, size_t end = 0) : here(input), index(start), end_index(end) {};
			CDAL_Iter(const CDAL_Iter& src) : here(src.here), index(src.index), end_index(src.end_index) {};

			//Functions
			reference operator*() const {		//TESTED
				if (used_slots + index >= end_index)
					throw std::runtime_error("Error: Past the end of the list");
				return here->array[index]; };

			pointer operator->() const {		//Tested
				if (used_slots + index >= end_index)
					throw std::runtime_error("Error: Past the end of the list");
				return ( &(operator*()) );	};

			self_reference operator=(CDAL_Iter const& src){
				this->here = src.here;
				this->index = src.index;
				return *this;	};

			self_reference operator++(){
				if (used_slots + index >= end_index)
					return *this;
				if (index == 49) {
					used_slots += 50;
					here = here->next;
					index = 0;
				}
				else
					index++;
				return *this; };	//preincrement

			self_type operator++(int) {
				CDAL_Iter<dataL> temp(here, this->index, end_index);
				if (used_slots + index == end_index) {}
				else if (index == 49 && !(used_slots + index >= end_index)) {
					used_slots += 50;
					here = here->next;
					index = 0;
				}
				else
					this->index++;
				return temp;	}; // postincrement

			bool operator==(CDAL_Iter const& rhs ) const {				//TESTED
				return (rhs.here == here && rhs.index == index);	};

			bool operator!=(CDAL_Iter const& rhs) const {				//TESTED
				return !(rhs.here == here && rhs.index == index); 	};
		};
		
		//using size_t = std::size_t;
		using value_type = L;
		using iterator = CDAL_Iter<L>;
		using const_iterator = CDAL_Iter<L const>;
		
		iterator begin() {return CDAL_Iter<L>(head, 0, full_slots);}
		iterator end() {
			size_t temp = full_slots % 50;
			if (length() == 0)
				return CDAL_Iter<L>(head, 0, 0);
			if (!temp)
				return CDAL_Iter<L>(nullptr, 0, 0);
			return CDAL_Iter<L>(tail, temp, full_slots);
		}
		const_iterator begin() const {return CDAL_Iter<L const>(head, 0, full_slots);}
		const_iterator end() const {
			size_t temp = full_slots % 50;
			if (length() == 0)
				return CDAL_Iter<L const>(head, 0, 0);
			if (!temp) {
				return CDAL_Iter<L const>(tail->next, 0, 0);
			}
			return CDAL_Iter<L const>(tail, temp, full_slots);
		}
};

template <typename L>
CDAL<L>::CDAL() {
	head = new c_node<L>();
	tail = head;
	total_slots = 50;
	full_slots = 0;
}

template <typename L>
CDAL<L>::CDAL(const CDAL<L> &src) {
	L* temp = src.contents();
	full_slots = 0;
	total_slots = 50;
	head = new c_node<L>();
	tail = head;
	size_t size = src.length();
	for (size_t i = 0; i < size; i++)
		push_back(temp[i]);
	delete [] temp;
}

template <typename L>
CDAL<L>::CDAL(CDAL<L> &&src) noexcept {
	head = src.head;
	tail = src.tail;
	full_slots = src.full_slots;
	total_slots = src.total_slots;

	src.head = nullptr;
	src.tail = head;
	src.full_slots = 0;
	src.total_slots = 50;
}

template <typename L>
CDAL<L>::~CDAL() {
	curr = head;
	while (head){
		curr = head->next;
		delete [] head->array;
		delete head;
		head = curr;
	}
}

template <typename L>
CDAL<L>& CDAL<L>::operator=(const CDAL<L> &src) {
	if (this != &src) {
		L* temp = src.contents();
		this->clear();
		size_t size = src.length();
		for (size_t i = 0; i < size; i++)
			push_back(temp[i]);
		delete [] temp;
	}
	return *this;
}

template <typename L>
CDAL<L>& CDAL<L>::operator=(CDAL<L> &&src) {
	if (this != &src) {
		this->clear();
		delete [] head->array;
		delete head;
		head = src.head;
		tail = src.tail;
		full_slots = src.full_slots;
		total_slots = src.total_slots;
		src.head = nullptr;
		src.tail = head;
		src.full_slots = 0;
		src.total_slots = 50;
	}
	return *this;
}

template <typename L>
void CDAL<L>::insert(const L& element, size_t position) {
	if (position > full_slots) {
		throw std::runtime_error("Error: Cannot insert an element at a spot larger than the list size");
	}
	if (position == 0 && full_slots == 1) {
		L temp = head->array[0];
		clear();
		return temp;
	}	
	if (full_slots == total_slots)
		up_size();
	curr = tail;
	int i = full_slots;
	/*This algorithm starts at the last full spot of an array and moves everything over 1 position 
	until it reaches the spot where the new elementis being entered.  If a current spot is the beginning 
	of the array, then the spot equals the last spot in the array of the previous node, and the current pointer moves to the previous node*/
	for ( ; i > position; i--) {
		size_t temp = i % 50;
		if (temp)
			curr->array[temp] = curr->array[temp - 1];
		else {
			curr->array[0] = curr->previous->array[49];
			curr = curr->previous;
		}
	}
	curr->array[i % 50] = element;
	full_slots++;
}

template <typename L>
void CDAL<L>::push_back(const L& element) {
	insert(element, full_slots);
}

template <typename L>
void CDAL<L>::push_front(const L& element) {
	insert(element, 0);
}

template <typename L>
L CDAL<L>::replace(const L& element, size_t position) {
	if (position >= full_slots)
		throw std::runtime_error("Error: Trying to replace an element that does not exist");
	curr = head;
	size_t nodes_over = position / 50;
	for (size_t i = 0; i < nodes_over; i++) {
		curr = curr->next;
	}
	int holder = (position) % 50;
	L temp = curr->array[holder];
	curr->array[holder] = element;
	return temp;
}

template <typename L>
L CDAL<L>::remove(size_t position) {
	if (position >= full_slots)
		throw std::runtime_error("Error: Cannot remove an item that does not exist");
	if (position == 0 && full_slots == 1) {
		if (head->next) {
			delete [] head->next->array;
			delete head->next;
		}
		L temp = head->array[0];
		clear();
		return temp;
	}	
	else {
		curr = head;
		size_t nodes_over = position / 50;
		for (size_t i = 0; i < nodes_over; i++) {
			curr = curr->next;
		}
		L temp = curr->array[position % 50];
		
		while (position < full_slots) {
			curr->array[position % 50] = curr->array[(position + 1) % 50];
			if (!((position + 1) % 50) && curr->next) {
				curr->array[49] = curr->next->array[0];
				curr = curr->next;
			}
			position++;
		}

		full_slots--;
		if (full_slots % 50 == 0) {
			tail = tail->previous;
			down_size();
		}

		return temp;
	}
}

template <typename L>
L CDAL<L>::pop_back() {
	return remove(full_slots - 1);
}

template <typename L>
L CDAL<L>::pop_front() {
	return remove(0);
}

template <typename L>
L& CDAL<L>::item_at(size_t position) {
	if (position >= full_slots)
		throw std::runtime_error("Error: Cannot return an item at a position that does not exist");
	curr = head;
	size_t nodes_over = position / 50;
	for (size_t i = 0; i < nodes_over; i++) {
		curr = curr->next;
	}
	return curr->array[position % 50];
}

template <typename L>
const L& CDAL<L>::item_at(size_t position) const {
	if (position >= full_slots)
		throw std::runtime_error("Error: Cannot return an item at a position that does not exist");
	cop3530::c_node<L>* temp = head;
	size_t nodes_over = position / 50;
	for (size_t i = 0; i < nodes_over; i++) {
		temp = temp->next;
	}
	return temp->array[position % 50];
}

template <typename L>
const L& CDAL<L>::peek_back() const {
	if (!full_slots) {
		throw std::runtime_error("Error: List is empty");
	}
	return tail->array[(full_slots - 1) % 50];
}

template <typename L>
L& CDAL<L>::peek_back() {
	if (!full_slots) {
		throw std::runtime_error("Error: List is empty");
	}
	return tail->array[(full_slots - 1) % 50];
}

template <typename L>
const L& CDAL<L>::peek_front() const {
	if (!full_slots) {
		throw std::runtime_error("Error: is empty");
	}
	return head->array[0];
}

template <typename L>
L& CDAL<L>::peek_front() {
	if (!full_slots) {
		throw std::runtime_error("Error: is empty");
	}
	return head->array[0];
}

template <typename L>
bool CDAL<L>::is_empty() const {
	return (!full_slots);
}

template <typename L>
bool CDAL<L>::is_full() const {
	return false;
}

template <typename L>
size_t CDAL<L>::length() const {
	return full_slots;
}

template <typename L>
void CDAL<L>::clear() {
	while (head) {
		curr = head->next;
		delete [] head->array;
		delete head;
		head = curr;
	}
	head = new c_node<L>();
	full_slots = 0;
	total_slots = 50;
	tail = head;
}

template <typename L>
std::ostream& CDAL<L>::print(std::ostream& output) const {
	if (!full_slots)
		output << "<empty list>";
	else {
		L* temp = contents();
		output << "[";
		for (size_t i = 0; i < full_slots - 1; i++)
			output << temp[i] << ",";
		output << temp[full_slots - 1] << "]";
	delete [] temp;
	}
	return output;	
}

template <typename L>
L* CDAL<L>::contents() const {
	if (!length())
		return nullptr;
	cop3530::c_node<L>* temp = head;
	L* return_array = new L[full_slots];
	size_t j = 0;
	for (size_t i = 0; i < full_slots; i++) {
		return_array[i] = temp->array[j];
		if (j == 49) {
			temp = temp->next;
			j = 0;
		}
		else
			j++;
	}
	return return_array;
}

template <typename L>
void CDAL<L>::up_size() {
	curr = new cop3530::c_node<L>();
	curr->previous = tail;
	tail->next = curr;
	tail = curr;
	total_slots += 50;
}

template <typename L>
void CDAL<L>::down_size() {
	if (tail && tail->next && tail->next->next) {
		delete [] tail->next->next->array;
		delete tail->next->next;
		tail->next->next = nullptr;
	}
}

template <typename L>
bool CDAL<L>::contains(const L& element, bool (*equals_function)(const L&, const L&)) const {
	c_node<L>* temp = head;
	int j = 0;
	for (int i = 0; i < full_slots; i++) {
		if (equals_function(element, temp->array[j]))
			return true;
		if (j == 49) {
			temp = temp->next;
			j = 0;
		}
		else
			j++;
	}
	return false;
}
}
#endif //CDAL.h