#ifndef CBL_H
#define CBL_H
#include "List.h"

namespace cop3530 {

template <typename L>
class CBL : public List<L> {
	public:
		CBL();
		CBL(size_t size);
		CBL(const CBL<L> &);
		CBL(CBL<L> &&) noexcept;
		~CBL();
		CBL<L>& operator=(const CBL<L> &);
		CBL<L>& operator=(CBL<L> &&);
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
		const L& item_at(size_t position) const override;					//Finished
		const L& peek_back() const override;									//Finished
		const L& peek_front() const override;								//Finished
		bool is_empty() const override;								//Finished
		bool is_full() const override;								//Finished
		size_t length() const override;							//verified
		void clear() override;									//Finished
		bool contains(const L& element, bool (*equals_function)(const L&, const L&)) const override;	//Finished
		std::ostream& print(std::ostream& output) const override;				//Finished
		L* contents() const override;									//Finished

	private:
		L* array;
		void up_size();								//FINISHED
		void down_size();							//FINISHED
		size_t tail = 1;			//Tells what index is last + 1
		size_t head = 0;
		size_t array_size = 50;
		size_t starting_capacity;

	public:
		//derived class	
		template <typename dataL>
		class CBL_Iter {
		private:
			dataL* array;
			size_t curr_index;
			size_t end_index;
			size_t array_end;

		public:	
			//The type aliases required for c++ iterator compatibility
			using value_type = dataL;
			using reference = dataL&;
			using pointer = dataL*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			
			//These are used to clarify the meaning of the code
			using self_type = CBL_Iter;
			using self_reference = CBL_Iter&;

			//Constructors
			explicit CBL_Iter(dataL* input = nullptr, size_t curr = 0, size_t end = 0, size_t last = 0) :
				array(input), curr_index(curr), end_index(end), array_end(last) {};
			CBL_Iter(const CBL_Iter& src) {*this = src;};

			pointer getArrayAddress() const {return array;};

			//Functions
			reference operator*() const {		//TESTED
				if (curr_index == end_index)
					throw std::runtime_error("Error: Past the end of the list");
				return array[curr_index]; };

			pointer operator->() const {		//Tested
				if (curr_index == end_index)
					throw std::runtime_error("Error: Past the end of the list");
				return ( &(operator*()) );	};

			self_reference operator=(CBL_Iter const& src){				//TESTED
				this->array = src.array;
				this->end_index = src.end_index;
				this->array_end = src.array_end;
				this->curr_index = src.curr_index;
				return *this;	};

			self_reference operator++(){					//TESTED
				if (curr_index == end_index)
					return *this;
				curr_index++;
				if (curr_index == array_end)
					curr_index = 0;
				return *this; };	//preincrement

			self_type operator++(int) {						//TESTED
				if (curr_index == end_index)
					return *this;
				CBL_Iter<dataL> temp(array, this->curr_index, this->end_index, this->array_end);
				this->curr_index++;
				if (curr_index == array_end)
					curr_index = 0;
				return temp;	}; // postincrement

			bool operator==(CBL_Iter const& rhs ) const {				//TESTED
				return (rhs.getArrayAddress() == this->getArrayAddress() && rhs.curr_index == this->curr_index);	};

			bool operator!=(CBL_Iter const& rhs) const {				//TESTED
				return !(rhs.getArrayAddress() == this->getArrayAddress() && rhs.curr_index == this->curr_index); 	};
		};
		
		//using size_t = std::size_t;
		using value_type = L;
		using iterator = CBL_Iter<L>;
		using const_iterator = CBL_Iter<L const>;
		
		iterator begin() {
			if (head == (array_size - 1))
				return CBL_Iter<L>(array, 0, tail, array_size);
			return CBL_Iter<L>(array, head + 1, tail, array_size);}
		iterator end() {return CBL_Iter<L>(array, tail, tail, array_size);}								//WHAT IF THIS IS OUT OF BOUNDS?? NEED TO ADD IN A CHECK FOR THAT TOMORROW
		const_iterator begin() const {
			if (head == (array_size - 1))
				return CBL_Iter<L const>(array, 0, tail, array_size);
			return CBL_Iter<L const>(array, head + 1, tail, array_size);}
		const_iterator end() const {return CBL_Iter<L const>(array, tail, tail, array_size);}					//NEED TO CHECK IF THIS AND PREVIOUS ONE ARE SUPPOSED TO BE TAIL TO TAIL + 1
};

template <typename L>
CBL<L>::CBL(size_t size) {
	array_size = size;
	array = new L[size];
	starting_capacity = size;
}

template <typename L>
CBL<L>::CBL() {
	array = new L[array_size];
	starting_capacity = array_size;
}

template <typename L>
CBL<L>::CBL(const CBL<L> &src) {
	tail = 1;
	head = 0;
	array_size = src.array_size;
	starting_capacity = src.starting_capacity;
	array = new L[array_size];
	L* temp = src.contents();
	size_t size = src.length();
	for (size_t i = 0; i < size; i++)
		this->push_back(temp[i]);
	delete [] temp;
}

template <typename L>
CBL<L>::CBL(CBL<L> &&src) noexcept {
	tail = src.tail;		src.tail = 1;
	head = src.head;		src.head = 0;
	array_size = src.array_size;
	starting_capacity = src.starting_capacity;
	array = src.array;
	src.array_size = src.starting_capacity;
	src.array = new L[starting_capacity];
}

template <typename L>
CBL<L>::~CBL() {
	delete [] array;
}

template <typename L>
CBL<L>& CBL<L>::operator=(const CBL<L> &src) {
	if (this != &src) {
		tail = 1;
		head = 0;
		array_size = src.array_size;
		starting_capacity = src.starting_capacity;
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
CBL<L>& CBL<L>::operator=(CBL<L> &&src) {
	if (this != &src) {
		delete [] this->array;
		tail = src.tail;		src.tail = 1;
		head = src.head;		src.head = 0;
		array_size = src.array_size;
		starting_capacity = src.starting_capacity;
		array = src.array;
		src.array_size = src.starting_capacity;
		src.array = new L[starting_capacity];
	}
	return *this;
}

template <typename L>
void CBL<L>::insert(const L& element, size_t position) {
	if (position > length())
		throw std::runtime_error("Error: Invalid Insertion position");
	if (tail == head)
		up_size();

	size_t p = head;
	for (size_t j = 0; j < position; j++) {
		if ( p == (array_size - 1)) {
			array[array_size - 1] = array[0];
			p = 0;
		}
		else {
			array[p] = array[p + 1];
			p++;
		}
	}
	array[p] = element;
	if (!head)
		head = array_size - 1;
	else
		head--;
}

template <typename L>
void CBL<L>::push_back(const L& element) {
	if (tail == head)
		up_size();
	array[tail] = element;
	tail++;
	if (tail == array_size)
		tail = 0;
}

template <typename L>
void CBL<L>::push_front(const L& element) {
	if (tail == head)
		up_size();
	array[head] = element;
	if (!head)
		head = array_size - 1;
	else
		head--;
}

template <typename L>
L CBL<L>::replace(const L& element, size_t position) {
	if (position >= length())
		throw std::runtime_error("Error: Invalid replacement position");
	size_t j = head + position + 1;
	if (j >= array_size)
		j -= array_size;
	L temp = array[j];
	array[j] = element;
	return temp;
}

template <typename L>
L CBL<L>::remove(size_t position) {
	if (position >= length())
		throw std::runtime_error("Error: Invalid position to remove item");

	size_t j = head + 1 + position;
	if (j >= array_size)
		j -= array_size;
	L temp = array[j];
	while (j != tail) {
		if (j == (array_size - 1)) {
			array[j] = array[0];
			j = 0;
		}
		else {
			array[j] = array[j + 1];
			j++;
		}
	}
	if (!tail)
		tail = array_size - 1;
	else
		tail--;
	//This deals with downsizing the circular array
	size_t k = tail;
	if (tail < head)
		k += array_size;
	size_t full_slots = k - head;
	if (array_size > 100 && full_slots <= (array_size / 2))
		down_size();
	return temp;
}

template <typename L>
L CBL<L>::pop_back() {
	return remove(length() - 1);
}

template <typename L>
L CBL<L>::pop_front() {
	return remove(0);
}

template <typename L>
L& CBL<L>::item_at(size_t position) {
	if (position >= length())
		throw std::runtime_error("Error: Trying to view invalid position");
	size_t j = head + position + 1;
	if (j >= array_size)
		j -= array_size;
	return array[j];
}

template <typename L>
const L& CBL<L>::item_at(size_t position) const {
	if (position >= length())
		throw std::runtime_error("Error: Trying to view invalid position");
	size_t j = head + position + 1;
	if (j >= array_size)
		j -= array_size;
	return array[j];
}

template <typename L>
L& CBL<L>::peek_back() {
	if (is_empty())
		throw std::runtime_error("Error: Trying to peek an empty list");
	if (!tail)
		return array[array_size - 1];
	return array[tail - 1];
}

template <typename L>
const L& CBL<L>::peek_back() const {
	if (is_empty())
		throw std::runtime_error("Error: Trying to peek an empty list");
	if (!tail)
		return array[array_size - 1];
	return array[tail - 1];
}

template <typename L>
L& CBL<L>::peek_front() {
	if (is_empty())
		throw std::runtime_error("Error: Trying to peek an empty list");
	if (head == array_size - 1)
		return array[0];
	return array[head + 1];
}

template <typename L>
const L& CBL<L>::peek_front() const {
	if (is_empty())
		throw std::runtime_error("Error: Trying to peek an empty list");
	if (head == array_size - 1)
		return array[0];
	return array[head + 1];
}

template <typename L>
bool CBL<L>::is_empty() const {
	return (( (head + 1) == tail ) || (head == (array_size - 1) && tail == 0));
}

template <typename L>
bool CBL<L>::is_full() const {
	return false;
}

template <typename L>
size_t CBL<L>::length() const {
	size_t j = tail;
	if (tail <= head)
		return (array_size - ((head - tail) + 1));
	return ((tail - head) - 1);
}

template <typename L>
void CBL<L>::clear() {
	tail = 1;
	head = 0;
	delete [] array;
	array = new L[starting_capacity];
	array_size = starting_capacity;
}

template <typename L>
std::ostream& CBL<L>::print(std::ostream& output) const {
	if (is_empty())
		output << "<empty list>";
	else {
		size_t size = length();
		L* temp = contents();
		output << "[";
		for (size_t i = 0; i < size - 1; i++)
			output << temp[i] << ",";
		output << temp[size - 1] << "]";
		delete [] temp;
	}
	return output;
}

template <typename L>
L* CBL<L>::contents() const {
	if (!length())
		return nullptr;
	L* temp = new L[length()];
	size_t j = head;
	size_t size = length();
	for (int i = 0; i < size; i++) {
		if (j == array_size - 1)
			j = 0;
		else
			j++;
		temp[i] = array[j];
	}
	return temp;
}

template <typename L>
void CBL<L>::up_size() {
	size_t size = length();	
	L* temp = array;
	array = new L[array_size * 2];
	for (size_t i = 0; i < size; i++) {
		if (head == array_size - 1)
			head = 0;
		else {
			head++;
		}
		array[i + 1] = temp[head];
	}
	array_size *= 2;
	head = 0;	tail = size + 1;
	delete [] temp;
}

template <typename L>
void CBL<L>::down_size() {
	size_t size = length();
	size_t holder = (array_size / 4) * 3;
	L *temp = new L[holder];
	for (size_t i = 0; i < holder; i++)
		temp[i] = L();

	L *prev_array = contents();
	for (size_t i = 1; i < size + 1; i++)
		temp[i] = prev_array[i - 1];
	array_size = holder;
	head = 0;		tail = size + 1;
	if (tail >= array_size)
		tail = 0;
	delete [] array;
	delete [] prev_array;
	array = temp;
}

template <typename L>
bool CBL<L>::contains(const L& element, bool (*equals_function)(const L&, const L&)) const {
	size_t size = length();
	size_t j = head;
	for (size_t i = 0; i < size; i++) {
		if (j == array_size - 1)
			j = 0;
		else
			j++;
		if (equals_function(element, array[j]))
			return true;
	}
	return false;
}

}
#endif //LIST_h