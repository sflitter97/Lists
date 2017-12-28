#ifndef C_NODE_H
#define C_NODE_H

namespace cop3530 {

	template <typename L>
	struct c_node	{
		c_node<L>() {};
		L* array = new L[50];
		c_node<L>*previous = nullptr;
		c_node<L>* next = nullptr;
	};
}

#endif //SSL_node