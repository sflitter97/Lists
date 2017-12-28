#ifndef NODE_H
#define NODE_H
//this comment is to test my git abilities yooo

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