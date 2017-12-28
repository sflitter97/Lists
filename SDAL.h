#ifndef SDAL_H
#define SDAL_H
#include "List.h"

namespace cop3530 {

template <typename L>
class SDAL : public List<L> {
	public:
		SDAL();
		SDAL(size_t size);
		SDAL(const SDAL<L> &);
		SDAL(SDAL<L> &&) noexcept;
		~SDAL();
		SDAL<L>& operator=(const SDAL<L> &);
		SDAL<L>& operator=(SDAL<L> &&);
		void insert(const L& element, size_t position) override;		//Finished
		void push_back(const L& element) override;						//Finished
		void push_front(const L& element) override;					//Finished
		L replace(const L& element, size_t position) override;			//Finished
		L remove(size_t position) override;						//Finished
		L pop_back() override;									//Finished
		L pop_front() override;									//Finished
		L& item_at(size_t position) override;					//Finished
		L& peek_back() override;									//Finished
		L& peek_front() override;								//Finished
		const L& item_at(size_t position) const override;
		const L& peek_back() const override;
		const L& peek_front() const override;
		bool is_empty() const override;								//Finished
		bool is_full() const override;								//Finished
		size_t length() const override;							//Finished
		void clear() override;									//Finished
		bool contains(const L& element, bool (*equals_function)(const L&, const L&)) const override;	//Finished
		std::ostream& print(std::ostream& output) const override;				//Finished
		L* contents() const override;									//Finished

	private:
		L* array;
		void up_size();								//FINISHED
		void down_size();							//FINISHED
		size_t tail = 0;			//Tells what index is last + 1
		size_t array_size = 50;
		size_t starting_capacity = 50;

	public:
		//derived class	
		template <typename dataL>
		class SDAL_Iter {
		private:
			dataL* array = nullptr;
			size_t length = 0;
			size_t index = 0;

		public:	
			//The type aliases required for c++ iterator compatibility
			using value_type = dataL;
			using reference = dataL&;
			using pointer = dataL*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			
			//These are used to clarify the meaning of the code
			using self_type = SDAL_Iter;
			using self_reference = SDAL_Iter&;

			//Constructors
			explicit SDAL_Iter(dataL* input = nullptr, size_t num = 0, size_t start = 0) : array(input), length(num), index(start) {};
			SDAL_Iter(const SDAL_Iter& src) : array(src.array), length(src.length), index(src.index) {};

			//Functions
			reference operator*() const {		//TESTED
				if (index >= length)
					throw std::runtime_error("Error: Past the end of the list");
				return array[index]; }

			pointer operator->() const {		//Tested
				if (index >= length)
					throw std::runtime_error("Error: Past the end of the list");
				return ( &(operator*()) );	};

			self_reference operator=(SDAL_Iter const& src){				//TESTED
				this->array = src.array;
				this->index = src.index;
				return *this;	};

			self_reference operator++(){					//TESTED
				if (this->index < this->length)
					index++;
				return *this; };	//preincrement

			self_type operator++(int) {						//TESTED
				SDAL_Iter<dataL> temp(array, length, this->index);
				if (this->index < this->length)
					this->index++;
				return temp;	}; // postincrement

			bool operator==(SDAL_Iter<dataL> const& rhs ) const {				//TESTED
				return (this->array == rhs.array && index == rhs.index); };

			bool operator!=(SDAL_Iter<dataL> const& rhs) const {				//TESTED
				return (array != rhs.array || index != rhs.index); };
		};
		
		//using size_t = std::size_t;
		using value_type = L;
		using iterator = SDAL_Iter<L>;
		using const_iterator = SDAL_Iter<L const>;
		
		iterator begin() {return SDAL_Iter<L>(array, tail, 0);}
		iterator end() {return SDAL_Iter<L>(array, tail, tail);}
		const_iterator begin() const {return SDAL_Iter<L const>(array, tail, 0);}
		const_iterator end() const {return SDAL_Iter<L const>(array, tail, tail);}
};

template <typename L>
SDAL<L>::SDAL(size_t size) {
	array_size = size;
	starting_capacity = size;
	array = new L[size];
}

template <typename L>
SDAL<L>::SDAL() {
	array = new L[50];
}

template <typename L>
SDAL<L>::SDAL(const SDAL<L> &src) {
	tail = 0;
	array_size = src.array_size;
	starting_capacity = src.starting_capacity;
	array = new L[array_size];
	L* temp = src.contents();
	for (size_t i = 0; i < src.tail; i++)
		this->push_back(temp[i]);
	delete [] temp;
}

template <typename L>
SDAL<L>::SDAL(SDAL<L> &&src) noexcept {
	starting_capacity = src.starting_capacity;
	array = src.array;
	tail = src.tail;
	array_size = src.array_size;
	starting_capacity = src.starting_capacity;
	src.array_size = src.starting_capacity;
	src.tail = 0;
	src.array = new L[starting_capacity];
}

template <typename L>
SDAL<L>::~SDAL() {
	delete [] array;
}

template <typename L>
SDAL<L>& SDAL<L>::operator=(const SDAL<L> &src) {
	if (this != &src) {
		tail = 0;
		array_size = src.array_size;
		starting_capacity = src.starting_capacity;
		this->clear();
		L* temp = src.contents();
		for (size_t i = 0; i < src.tail; i++)
			this->push_back(temp[i]);
		delete [] temp;
	}
	return *this;
}

template <typename L>
SDAL<L>& SDAL<L>::operator=(SDAL<L> &&src) {
	if (this != &src) {
		starting_capacity = src.starting_capacity;
		delete [] array;
		array = src.array;
		tail = src.tail;
		array_size = src.array_size;
		starting_capacity = src.starting_capacity;
		src.array_size = src.starting_capacity;
		src.tail = 0;
		src.array = new L[starting_capacity];
	}
	return *this;
}

template <typename L>
void SDAL<L>::insert(const L& element, size_t position) {
	if (position > tail)
		throw std::runtime_error("Error: Invalid Insertion position");
	if (tail == (array_size))
		up_size();
	for (int i = tail; i > position; i--) {
		array[i] = array[i - 1];
	}
	array[position] = element;
	tail++;
}

template <typename L>
void SDAL<L>::push_back(const L& element) {
	insert(element, tail);
}

template <typename L>
void SDAL<L>::push_front(const L& element) {
	insert(element, 0);
}

template <typename L>
L SDAL<L>::replace(const L& element, size_t position) {
	if (position >= tail)
		throw std::runtime_error("Error: Invalid position");
	L temp = array[position];
	array[position] = element;
	return temp;
}

template <typename L>
L SDAL<L>::remove(size_t position) {
	if (position >= tail)
		throw std::runtime_error("Error: Invalid position to remove item");

	L temp = array[position];
	for (size_t i = position; i < tail - 1; i++) {
		array[i] = array[i + 1];
	}
	tail--;

	if (array_size > 2 * starting_capacity && tail <= (array_size / 2))
		down_size();
	return temp;
}

template <typename L>
L SDAL<L>::pop_back() {
	return remove(tail - 1);
}

template <typename L>
L SDAL<L>::pop_front() {
	return remove(0);
}

template <typename L>
L& SDAL<L>::item_at(size_t position) {
	if (position >= tail)
		throw std::runtime_error("Error: Trying to view invalid position");
	return array[position];
}

template <typename L>
const L& SDAL<L>::item_at(size_t position) const {
	if (position >= tail)
		throw std::runtime_error("Error: Trying to view invalid position");
	return array[position];
}

template <typename L>
const L& SDAL<L>::peek_back() const {
	if (tail < 1)
		throw std::runtime_error("Error: Trying to peek an empty list");
	return array[tail - 1];
}

template <typename L>
L& SDAL<L>::peek_back() {
	if (tail < 1)
		throw std::runtime_error("Error: Trying to peek an empty list");
	return array[tail - 1];
}

template <typename L>
const L& SDAL<L>::peek_front() const {
	if (tail < 1)
		throw std::runtime_error("Error: Trying to peek an empty list");
	return array[0];
}

template <typename L>
L& SDAL<L>::peek_front() {
	if (tail < 1)
		throw std::runtime_error("Error: Trying to peek an empty list");
	return array[0];
}

template <typename L>
bool SDAL<L>::is_empty() const {
	return (tail == 0);
}

template <typename L>
bool SDAL<L>::is_full() const {
	return false;
}

template <typename L>
size_t SDAL<L>::length() const {
	return tail;
}

template <typename L>
void SDAL<L>::clear() {
	tail = 0;
	delete [] array;
	array = new L[starting_capacity];
	array_size = starting_capacity;
}

template <typename L>
std::ostream& SDAL<L>::print(std::ostream& output) const {
	if (is_empty())
		output << "<empty list>";
	else {
		output << "[";
		int i = 0;
		while (i < tail - 1) {
			output << array[i];
			output << ",";
			i++;
		}
		output << array[i] << "]";
	}
	return output;	
}

template <typename L>
L* SDAL<L>::contents() const {
	if (!length())
		return nullptr;
	L* temp = new L[tail];
	for (int i = 0; i < tail; i++) {
		temp[i] = array[i];
	}
	return temp;
}

template <typename L>
void SDAL<L>::up_size() {
	L *temp = new L[array_size * 2];
	array_size *= 2;
	for (int i = 0; i < tail; i++) {
		temp[i] = array[i];
	}
	delete [] array;
	array = temp;
}

template <typename L>
void SDAL<L>::down_size() {
	L *temp = new L[(array_size / 4) * 3];
	array_size = (array_size / 4) * 3;
	for (int i = 0; i <= tail; i++) {
		temp[i] = array[i];
	}
	delete [] array;
	array = temp;
}

template <typename L>
bool SDAL<L>::contains(const L& element, bool (*equals_function)(const L&, const L&)) const {
	for (int i = 0; i < tail; i++) {
		if (equals_function(element, array[i]))
			return true;
	}
	return false;
}

}
#endif //LIST_h