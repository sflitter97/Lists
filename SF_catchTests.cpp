#define CATCH_CONFIG_MAIN
#include "catch.hpp"
//#include "SSLL.h"
//#include "PSLL.h"
//#include "SDAL.h"
//#include "CBL.h"
#include "CDAL.h"
#include <sstream>

#ifndef List_Type
#define List_Type SDAL
#endif

bool compare_int(const int& x, const int& y);


TEST_CASE("0-state int checks") {
	cop3530::List_Type<int>* test1 = new cop3530::List_Type<int>();
	
	SECTION("Before any operations have been performed") {
		REQUIRE(test1->is_empty());
		REQUIRE(!test1->is_full());
		REQUIRE(!test1->length());
		REQUIRE(!test1->contents());
		std::stringstream temp;		test1->print(temp);
		REQUIRE(temp.str() == "<empty list>");
		REQUIRE(!test1->contains(0, compare_int));
	}

	SECTION("Adding and then removing elements to check 0-state") {
		test1->push_front(4);
		test1->push_back(5);
		REQUIRE(!test1->is_empty());
		REQUIRE(!test1->is_full());
		test1->pop_back();
		test1->pop_front();
		REQUIRE(test1->is_empty());
		REQUIRE(!test1->is_full());
		REQUIRE(!test1->length());
		REQUIRE(!test1->contents());
		std::stringstream temp;		test1->print(temp);
		REQUIRE(temp.str() == "<empty list>");
		REQUIRE(!test1->contains(0, compare_int));
	}

	SECTION("Exception Handling") {
		REQUIRE_THROWS(test1->replace(5, 2));
		REQUIRE_THROWS(test1->replace(4, 0));
		REQUIRE_THROWS(test1->remove(3));
		REQUIRE_THROWS(test1->remove(0));
		REQUIRE_THROWS(test1->pop_back());
		REQUIRE_THROWS(test1->pop_front());
		REQUIRE_THROWS(test1->peek_back());
		REQUIRE_THROWS(test1->peek_front());
		REQUIRE_THROWS(test1->item_at(0));
		test1->push_front(3);	test1->push_back(4);
		REQUIRE_THROWS(test1->item_at(4));
		REQUIRE_THROWS(test1->item_at(2));
		REQUIRE_THROWS(test1->remove(2));
		REQUIRE_THROWS(test1->replace(2, 2));
	}
	delete test1;
}

TEST_CASE("K-th state int check") {
	cop3530::List_Type<int>* test = new cop3530::List_Type<int>();

	SECTION("Viewing items") {
		for (size_t i = 0; i < 400; i++) {
			test->push_back(i);
			REQUIRE(test->peek_back() == i);
			REQUIRE(test->peek_front() == 0);
		}
		for (size_t i = 0; i < 400; i++)
			REQUIRE(test->item_at(i) == i);
		for (size_t i = 0; i < 400; i++) {
			REQUIRE(test->peek_front() == i);
			REQUIRE(test->peek_back() == 399);
			test->pop_front();
		}
	}

	SECTION("Adding elements/checking item_at") {
		for (size_t i = 0; i < 100; i++) {
			test->push_front(i);
			REQUIRE(test->item_at(0) == i);
		}
		for (size_t i = 0; i < 100; i++) {
			test->push_back(i);
			REQUIRE(test->item_at(100 + i) == i);
		}
		for (size_t i = 25; i < 125; i++) {
			test->insert(300 + i, i);
			REQUIRE(test->item_at(i) == 300 + i);
		}
		test->item_at(50) = 30;
		REQUIRE(test->item_at(50) == 30);
		test->item_at(50) = 50;
	}

	SECTION("Removing elements from list") {
		for (size_t i = 0; i < 300; i++)
			test->push_back(i);
		for (size_t i = 0; i < 50; i++)
			REQUIRE(test->remove(150) == 150 + i);
		for (size_t i = 0; i < 50; i++)
			REQUIRE(test->remove(100) == 100 + i);
		for (size_t i = 0; i < 100; i++)
			REQUIRE(test->pop_front() == i);
		for (size_t i = 0; i < 100; i++)
			REQUIRE(test->pop_back() == 299 - i);
	}

	SECTION("Length Tests") {
		for (size_t i = 0; i < 300; i++) {
			REQUIRE(test->length() == i);
			test->push_back(i);
		}
		for (size_t i = 300; i > 0; i--) {
			REQUIRE(test->length() == i);
			test->pop_front();
		}
		REQUIRE(!test->length());
	}

	SECTION("Replace, Contains, Contents") {
		for (size_t i = 0; i < 200; i++)
			test->push_back(i);
		//Contains Tests
		REQUIRE(test->contains(57, compare_int));
		REQUIRE(!test->contains(200, compare_int));
		//Contents Tests
		int* temp = test->contents();
		for (size_t i = 0; i < 200; i++)
			REQUIRE(temp[i] == i);
		delete [] temp;
		//Replace Tests
		for (size_t i = 0; i < 200; i++) {
			REQUIRE(test->replace(300, i) == i);
			REQUIRE(test->item_at(i) == 300);
		}
	}

	SECTION("Clear") {
		for (size_t i = 0; i < 400; i++)
			test->insert(i, i);
		test->clear();
		REQUIRE(!test->length());
		REQUIRE(test->is_empty());
		REQUIRE(!test->contents());
	}

	SECTION("Print") {
		for (size_t i = 0; i < 100; i++)
			test->push_back(i);
		std::stringstream temp1;		test->print(temp1);
		std::stringstream temp2;
		temp2 << "[";
		for (size_t i = 0; i < 99; i++) {
			temp2 << i;
			temp2 << ",";
		}
		temp2 << 99;
		temp2 << "]";
		REQUIRE(temp1.str() == temp2.str());
	}
	delete test;
}

TEST_CASE("Iterators") {
	cop3530::List_Type<int>* test = new cop3530::List_Type<int>();
	for (size_t i = 0; i < 250; i++)
		test->push_back(i);
	cop3530::List_Type<int>::iterator list_iter = test->begin();
	cop3530::List_Type<int>::iterator list_end = test->end();

	SECTION("Postincrement and Dereference") {
		for (size_t i = 0; i < 250; i++)
			REQUIRE(*list_iter++ == i);
	}
	SECTION("Preincrement and Dereference") {
		for (size_t i = 0; i < 249; i++)
			REQUIRE(*++list_iter == i + 1);
	}
	SECTION("Enhanced for-loops and Dereference") {
		size_t i = 0;
		for (int here : *test)
			REQUIRE(here == test->item_at(i++));
	}
	SECTION("Equality, non-equality and Assignment operations") {
		cop3530::List_Type<int>* new_test = new cop3530::List_Type<int>();
		cop3530::List_Type<int>::iterator new_iter = new_test->begin();
		REQUIRE(list_iter != new_iter);
		REQUIRE(!(list_iter == new_iter));
		for (size_t i = 0; i < 250; i++)
			new_test->push_back(i);
		new_iter = new_test->begin();
		REQUIRE(list_iter != new_iter);
		REQUIRE(!(list_iter == new_iter));
		new_iter = list_iter;
		REQUIRE(new_iter == list_iter);
		REQUIRE(!(new_iter != list_iter));
		delete new_test;
	}

	SECTION("Arrow Operator") {
		cop3530::List_Type<std::string>* new_test = new cop3530::List_Type<std::string>();
		new_test->push_back("Dave is great");
		cop3530::List_Type<std::string>::iterator new_iter = new_test->begin();
		REQUIRE(new_iter->size() == 13);
		delete new_test;
	}

	SECTION("Accessing an iterator that is out of bounds") {
		for (size_t i = 0; i < 250; i++)
			list_iter++;
		REQUIRE_THROWS(*list_iter);
		cop3530::List_Type<std::string>* new_test = new cop3530::List_Type<std::string>();
		new_test->push_front("Last test yo");
		cop3530::List_Type<std::string>::iterator new_iter = new_test->begin();
		new_iter++;
		REQUIRE_THROWS(new_iter->size());
		delete new_test;
	}
	delete test;
}

TEST_CASE("Assignment Constructors/Operators") {
	cop3530::List_Type<int>* primary = new cop3530::List_Type<int>();
	for (size_t i = 0; i < 250; i++)
		primary->push_back(i);

	SECTION("Copy Constructor") {
		cop3530::List_Type<int>* test = new cop3530::List_Type<int>(*primary);
		REQUIRE(primary != test);
		for (size_t i = 0; i < 250; i++)
			REQUIRE(primary->item_at(i) == test->item_at(i));
		test->pop_front();
		REQUIRE(test->pop_front() != primary->pop_front());
		delete test;
	}

	SECTION("Copy Assignment Operator") {
		cop3530::List_Type<int>* test = new cop3530::List_Type<int>();
		REQUIRE(primary != test);
		*test = *primary;
		*test = *test;
		for (size_t i = 0; i < 250; i++)
			REQUIRE(primary->item_at(i) == test->item_at(i));
		test->pop_front();
		REQUIRE(test->pop_front() != primary->pop_front());
		delete test;
	}

	SECTION("Move Constructor") {
		cop3530::List_Type<int>* test = new cop3530::List_Type<int>(std::move(*primary));
		REQUIRE(primary != test);
		for (size_t i = 0; i < 250; i++)
			REQUIRE(test->item_at(i) == i);
		REQUIRE(!primary->length());
		delete test;
	}

	SECTION("Move Operator") {
		cop3530::List_Type<int>* test = new cop3530::List_Type<int>();
		*test = std::move(*primary);
		REQUIRE(primary != test);
		*test = std::move(*test);
		for (size_t i = 0; i < 250; i++)
			REQUIRE(test->item_at(i) == i);
		REQUIRE(!primary->length());
		delete test;
	}
	delete primary;
}

bool compare_int(const int& x, const int& y) {
	return (x == y);
}