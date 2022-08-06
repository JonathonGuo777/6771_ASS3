#include "gdwg/graph.hpp"
#include <catch2/catch.hpp>
#include <vector>



TEST_CASE("Empty") {
	auto const g_empty = gdwg::graph<std::string, int>{};
	CHECK(g_empty.begin() == g_empty.end());
}
TEST_CASE("Iterator "){
	auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
	g.insert_edge("a", "b", 100);
	g.insert_edge("a", "a", 50);
	g.insert_edge("c", "b", 200);
	auto const g_const = g;
	SECTION("Begin and end"){
		auto  it = g_const.begin();
		CHECK((*it).from == "a");
		CHECK((*it).to == "a");
		CHECK((*it).weight == 50);

		auto g2 = gdwg::graph<std::string, int>{"a", "b", "text"};
		CHECK(g2.begin() == g2.end());
	}

		SECTION("Operator++(int)"){
		auto it = g_const.begin();
		CHECK((*it).from == "a");
		CHECK((*it).to == "a");
		CHECK((*it).weight == 50);
		++it;
		CHECK((*it).from == "a");
		CHECK((*it).to == "b");
		CHECK((*it).weight == 100);
		++it;
		CHECK((*it).from == "c");
		CHECK((*it).to == "b");
		CHECK((*it).weight == 200);
		++it;
		CHECK(it == g_const.end());
	}
	SECTION("Operator--(int)"){
		auto it = g_const.end();
		--it;
		CHECK((*it).from == "c");
		CHECK((*it).to == "b");
		CHECK((*it).weight == 200);
		--it;
		CHECK((*it).from == "a");
		CHECK((*it).to == "b");
		CHECK((*it).weight == 100);
		--it;
		CHECK((*it).from == "a");
		CHECK((*it).to == "a");
		CHECK((*it).weight == 50);
		CHECK(it == g_const.begin());
	}

	SECTION("Equal"){
		auto g2 = gdwg::graph<int, std::string>();
		CHECK(g.begin() == g.begin());
		CHECK(g2.begin() == g2.end());
	}
}

