#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
// equals
TEST_CASE("equals_test") {
	using graph = gdwg::graph<int, int>;
	const auto g1 = graph();
	const auto g2 = graph();
	auto g3 = graph{3,2};
	g3.insert_edge(3,2,4);
	const auto g4 = graph(g3);
	CHECK(g1 == g2);
	CHECK(!(g1 == g3));
	CHECK(g3 == g4);
	CHECK(g3.weights(3,2) == g4.weights(3,2));
}

// print
TEST_CASE("default_constructor_ints") {
	using graph = gdwg::graph<int, int>;
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
	for (const auto &x : v) {
		g.insert_node(x.from);
		g.insert_node(x.to);
		g.insert_edge(x.from, x.to, x.weight);
	};

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