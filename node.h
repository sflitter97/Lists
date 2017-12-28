#ifndef NODE_H
#define NODE_H

namespace cop3530 {

	template <typename L>
	struct node	{
		node<L>() {};
		node<L>(L value, node<L>* following = nullptr) : data(value), next(following) {};
		L data;
		node<L>* next = nullptr;
	};
}

#endif //SSL_node