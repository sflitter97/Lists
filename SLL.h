#ifndef SLL_H
#define SLL_H
#include "List.h"
#include "node.h"

namespace cop3530 {

template <typename L>
class SLL : public List<L> {

public:
	SLL();				//Finished
	~SLL();			//FINISHED
	void insert(const L& element, size_t position) override;	//Finished		Updated
	void push_front(const L& element) override;				//Finished
	void push_back(const L& element) override;					//Finished	
	L replace(const L& element, size_t position) override;	//Finished		Updated
	L remove(size_t position) override;					//Finished
	L pop_back() override;					//Finished
	L pop_front() override;					//Finished
	L& item_at(size_t position) override;	//Finished
	L& peek_back() override;					//Finished
	L& peek_front() override;				//Finished
	const L& item_at(size_t position) const override;
	const L& peek_back() const override;
	const L& peek_front() const override;
	bool is_empty() const override;				//Finished
	bool is_full() const override;				//Finished
	size_t length() const override;				//Finished			
	void clear() override;					//Finished
	bool contains(const L& element, bool (*equals_function)(const L&, const L&)) const override;	//Finished
	std::ostream& print(std::ostream& output) const override;				//Finished
	L* contents() const override;				//Finished

/*	struct node	{
		node() {};
		node(L value, node<L>* following = nullptr) : data(value), next(following) {};
		L data;
		node<L>* next = nullptr;
	};*/
protected:
	cop3530::node<L>* head;
	cop3530::node<L>* tail;
	cop3530::node<L>* curr;
	virtual node<L>* add_node() = 0;					//Helper Method
	virtual void delete_node(cop3530::node<L>* erased) = 0;		//Helper Method
};

	template <class L>
	SLL<L>::~SLL() {
		while (head) {
			curr = head;
			head = head->next;
			delete curr;
		}
	}

	template <class L>
	SLL<L>::SLL() {
		head = tail = nullptr;
	}

	template <class L>
	void SLL<L>::insert(const L& element, size_t position) {
		if (position > length())
			throw std::runtime_error("Error: invalid position");
		else if (!head) {
			head = tail = add_node();
			head->data = element;
		}
		else if (!position) {
			node<L>* temp = head;
			head = add_node();
			head->data = element;
			head->next = temp;
		}
		else {
			curr = head;
			for (int i = 1; i < position; i++) {
				curr = curr->next;
			}
			cop3530::node<L>* temp = curr->next;
			curr->next = add_node();
			curr->next->data = element;
			curr->next->next = temp;
			if (curr == tail) 
				tail = tail->next;
		}
	}

	template <typename L>
	void SLL<L>::push_back(const L& element) {
		if (!is_empty()) {
			tail->next = add_node();
			tail->next->data = element;
			tail = tail->next;
			tail->next = nullptr;
		}
		else {
			head = tail = add_node();
			head->data = element;
			head->next = nullptr;
		}
	}

	template <typename L>
	void SLL<L>::push_front(const L& element) {
		if (!is_empty()) {
			node<L>* temp = head;
			head = add_node();
			head->data = element;
			head->next = temp;
		}
		else {
			head = add_node();
			head->data = element;
			head->next = nullptr;
			tail = head;
		}
	}

	template <class L>
	L SLL<L>::replace(const L& element, size_t position) {
		if (position >= length())
			throw std::runtime_error("Error: The list is not long enough to add an element at that location");
		curr = head;
		for (size_t i = 0; i < position; i++) {
			curr = curr->next;
		}
		L temp = curr->data;
		curr->data = element;
		return temp;
	}

	template <class L>
	L SLL<L>::remove(size_t position) {
		if (position >= length())
			throw std::runtime_error("Error: Cannot remove an element that does not exist");
		if (position != 0) {
			curr = head;
			for (size_t i = 0; i < position - 1; i++) {
				curr = curr->next;
			}
			if (!curr->next->next)
				tail = curr;
			node<L>* temp = curr->next;
			L temp_data = curr->next->data;
			curr->next = curr->next->next;
			delete_node(temp);
			return temp_data;
		}
		else {
			if (!(head && head->next))
				tail = nullptr;
			node<L>* temp = head;
			L temp_data = head->data;
			head = head->next;
			delete_node(temp);
			return temp_data;
		}
	}

	template <class L>
	L SLL<L>::pop_back() {
		if (is_empty())
			throw std::runtime_error("Error: Cannot pop from an empty list");
		L temp_data = tail->data;
		node<L>* temp_node = tail;
		tail = head;
		if (!head->next) {head = nullptr; tail = nullptr;}
		else {
			while (tail->next->next) {
				tail = tail->next;
			}
			tail->next = nullptr;			
		}
		delete_node(temp_node);
		return (temp_data);
	}

	template <class L>
	L SLL<L>::pop_front() {
		if (is_empty())
			throw std::runtime_error("Error: Cannot pop from an empty list");
		L temp_data = head->data;
		node<L>* temp_node = head;
		head = head->next;
		delete_node(temp_node);
		if (!head) 
			tail = nullptr;
		return temp_data;
	}

	template <class L>
	const L& SLL<L>::item_at(size_t position) const {
		if (position >= length())
			throw std::runtime_error("Error: Trying to access element in a position that does not exist");
		node<L>* temp = head;
		for (size_t i = 0; i < position; i++)
			temp = temp->next;
		return temp->data;
	}

	template <class L>
	L& SLL<L>::item_at(size_t position) {
		if (position >= length())
			throw std::runtime_error("Error: Trying to access element in a position that does not exist");
		curr = head;
		for (size_t i = 0; i < position; i++)
			curr = curr->next;
		return curr->data;
	}

	template <class L>
	const L& SLL<L>::peek_back() const {
		if (is_empty())
			throw std::runtime_error("Error: The list is empty");
		return tail->data;
	}

	template <class L>
	L& SLL<L>::peek_back() {
		if (is_empty())
			throw std::runtime_error("Error: The list is empty");
		return tail->data;
	}

	template <class L>
	const L& SLL<L>::peek_front() const {
		if (is_empty())
			throw std::runtime_error("Error: The list is empty");
		return head->data;
	}

	template <class L>
	L& SLL<L>::peek_front() {
		if (is_empty())
			throw std::runtime_error("Error: The list is empty");
		return head->data;
	}

	template <class L>
	bool SLL<L>::is_empty() const {
		if (head)
			return false;
		return true;
	}

	template <class L>
	size_t SLL<L>::length() const{
		size_t length = 0;
		node<L>* temp = head;
		while (temp) {
			length++;
			temp = temp->next;
		}
		return length;
	}

	template <class L>
	void SLL<L>::clear() {
		curr = head;
		while (curr) {
			curr = curr->next;
			delete_node(head);
			head = curr;
		}
		tail = head = nullptr;
	}

	template <class L>
	std::ostream& SLL<L>::print(std::ostream& output) const {
		if (is_empty())
			output << "<empty list>";
		else {
			output << "[";
			node<L>* temp = head;
			while (temp->next) {
				output << temp->data;
				output << ",";
				temp = temp->next;
			}
			output << temp->data << "]";
		}
		return output;
	}

	template <class L>
	L* SLL<L>::contents() const {
		if (length() == 0)
			return nullptr;
		L* array = new L[length()];
		node<L>* temp = head;
		int i = 0;
		while (temp) {
			array[i++] = temp->data;
			temp = temp->next;
		}
		return array;
	}

	template <class L>
	bool SLL<L>::is_full() const {
		return false;
	}

	template <class L>
	bool SLL<L>::contains(const L& element, bool (*equals_function)(const L&, const L&)) const {
		node<L>* temp = head;
		while (temp) {
			if (equals_function(element, temp->data))
				return true;
			temp = temp->next;
		}
		return false;
	}

}
#endif //SLL_H