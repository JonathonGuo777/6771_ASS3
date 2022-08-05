#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

//  begin_end iterator
TEST_CASE("empty_graph_with_nodes") {
	using graph = gdwg::graph<int, int>;
	const auto g1 = graph();
	CHECK(g1.begin() == g1.end());
	const auto g2 = graph{1,2,3,4,5};
	CHECK(g2.nodes() == std::vector<int>{1,2,3,4,5});
	CHECK(g2.begin() == g2.end());

}

TEST_CASE("graph_with_edges") {
	using graph = gdwg::graph<int, int>;
	using value_type = gdwg::graph<int, int>::value_type;
	auto g = graph{1,2,3,4,5};
	CHECK(g.nodes() == std::vector<int>{1,2,3,4,5});
	CHECK(g.begin() == g.end());
	g.insert_edge(1,2,3);
	CHECK(*g.begin() == value_type{1,2,3});
}

// increment iterator
TEST_CASE("increment_post") {
	using graph = gdwg::graph<int, int>;
	using value_type = gdwg::graph<int,int>::value_type;
	auto g = graph{1,2,3,4,5};
	g.insert_edge(1,2,3);
	g.insert_edge(1,2,7);
	g.insert_edge(1,2,4);
	g.insert_edge(1,2,6);
	auto it = g.begin();
	CHECK(*it == value_type{1,2,3});
	it = it++;
	CHECK(*it == value_type{1,2,4});
}

TEST_CASE("increment_pre") {
	using graph = gdwg::graph<int, int>;
	using value_type = gdwg::graph<int,int>::value_type;
	auto g = graph{1,2,3,4,5};
	g.insert_edge(1,2,3);
	g.insert_edge(1,2,7);
	g.insert_edge(1,2,4);
	g.insert_edge(1,2,6);
	auto it = g.begin();
	CHECK(*it == value_type{1,2,3});
	++it;
	CHECK(*it == value_type{1,2,4});
}

// decrement iterator
TEST_CASE("decrement_pre") {
	using graph = gdwg::graph<int, int>;
	using value_type = gdwg::graph<int,int>::value_type;
	auto g = graph{1,2,3,4,5};
	g.insert_edge(1,2,3);
	g.insert_edge(1,2,7);
	g.insert_edge(1,2,4);
	g.insert_edge(1,2,6);
	auto it_original = g.begin();
	auto it = g.begin();
	CHECK(*it == value_type{1,2,3});
	it = it++;
	CHECK(*it == value_type{1,2,4});
	it = it--;
	CHECK(*it == value_type{1,2,3});
	CHECK(it == it_original);
	CHECK(it_original == g.begin());
}

TEST_CASE("decrement_post") {
	using graph = gdwg::graph<int, int>;
	using value_type = gdwg::graph<int,int>::value_type;
	auto g = graph{1,2,3,4,5};
	g.insert_edge(1,2,3);
	g.insert_edge(1,2,7);
	g.insert_edge(1,2,4);
	g.insert_edge(1,2,6);
	auto it_original = g.begin();
	auto it = g.begin();
	CHECK(*it == value_type{1,2,3});
	it = it++;
	CHECK(*it == value_type{1,2,4});
	--it;
	CHECK(*it == value_type{1,2,3});
	CHECK(it == it_original);
	CHECK(it_original == g.begin());
}

// equals operator
TEST_CASE("Equal_iterators") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4, 5};
	g.insert_edge(1, 2, 3);
	CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4, 5});
	auto first_it = g.begin();
	++first_it;
	auto second_it = g.begin();
	++second_it;
	CHECK(first_it == second_it);
	--second_it;
	CHECK(!(second_it == first_it));
	CHECK(second_it == g.begin());
}

// dereference operator
TEST_CASE("dereference_value") {
	using graph = gdwg::graph<int, int>;
	using value_type = gdwg::graph<int, int>::value_type;
	auto g = graph{1, 2, 3, 4, 5};
	g.insert_edge(1, 2, 3);
	CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4, 5});
	CHECK(*g.begin() == value_type{1, 2, 3});
}