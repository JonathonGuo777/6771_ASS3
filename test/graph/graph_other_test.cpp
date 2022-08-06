#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>


TEST_CASE("Comparisons") {
	auto g1 = gdwg::graph<std::string, int>{};
	auto g2 = gdwg::graph<std::string, int>{"a", "b", "c"};
	auto g3 = gdwg::graph<std::string, int>{"a", "b", "c"};

	// equal
	CHECK(g1 == g1);
	CHECK(g2 == g2);
	CHECK(g3 == g3);

	// not equal
	CHECK(g1 != g2);
	CHECK(g3 != g1);
   // insert edge
	g2.insert_edge("a", "b", 7);
	CHECK(g2 != g3);

	g3.insert_edge("a", "b", 7);
	CHECK(g2 == g3);
}


TEST_CASE("Extractor ") {
	using graph = gdwg::graph<int, double>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		g.insert_edge(from, to, weight);
	}
	g.insert_node(64);
	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
	CHECK(out.str() == expected_output);

}