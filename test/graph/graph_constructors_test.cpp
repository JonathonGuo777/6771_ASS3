#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>


// default constructor
TEST_CASE("default_constructor_ints") {
	using graph = gdwg::graph<int, int>;
	const auto g = graph();
	CHECK(g.empty());
}


// initializer list constructor
TEST_CASE("initializer_list_constructor") {
	using graph = gdwg::graph<std::string, int>;
	const auto vector_il = std::vector<std::string>{"Hey", "hello", "why", "??", "lol", "Kappa123", "PauseChamp"};
	const auto g = graph{"Hey", "hello", "why", "??", "lol", "Kappa123", "PauseChamp"};
	const auto all_nodes = g.nodes();
	auto sorted_nodes = vector_il;
	std::sort(sorted_nodes.begin(), sorted_nodes.end());
	// wasn't bothered to sort it manually so i let c++ do it for me
	CHECK(!g.empty());
	CHECK(std::is_sorted(all_nodes.begin(), all_nodes.end()));
	CHECK(std::equal(all_nodes.begin(), all_nodes.end(), sorted_nodes.begin()));
}

// copy constructor
TEST_CASE("copy_constructor") {
	using graph = gdwg::graph<std::string, int>;
	const auto g1 = graph{"Hey", "hello", "why", "??", "lol", "Kappa123", "PauseChamp"};
	CHECK(!g1.empty());
	const auto g2 = g1;
	CHECK(g1 == g2);
}

// copy equals operator
TEST_CASE("copy_equals") {
	using graph = gdwg::graph<std::string, int>;
	const auto g1 = graph{"Hey", "hello", "why", "??", "lol", "Kappa123", "PauseChamp"};
	CHECK(!g1.empty());
	const auto g2 = g1;
	CHECK(g1 == g2);
}


// move constructor
TEST_CASE("move_constructor") {
	using graph = gdwg::graph<std::string, int>;
	auto g1 = graph{"Hey", "hello", "why", "??", "lol", "Kappa123", "PauseChamp"};
	const auto all_nodes1 = g1.nodes();
	CHECK(!g1.empty());
	const auto g2 = graph(std::move(g1));
	const auto all_nodes2 = g2.nodes();
	CHECK(g1.empty());
	CHECK(std::equal(all_nodes1.begin(), all_nodes1.end(), all_nodes2.begin(), all_nodes2.end()));
}

// move equals operator
TEST_CASE("move_equals") {
	using graph = gdwg::graph<std::string, int>;
	auto g1 = graph{"Hey", "hello", "why", "??", "lol", "Kappa123", "PauseChamp"};
	const auto all_nodes1 = g1.nodes();
	CHECK(!g1.empty());
	const auto g2 = std::move(g1);
	const auto all_nodes2 = g2.nodes();
	CHECK(g1.empty());
	CHECK(std::equal(all_nodes1.begin(), all_nodes1.end(), all_nodes2.begin(), all_nodes2.end()));
}

// input iterator constructor
TEST_CASE("input_iterator_constructor") {
	using graph = gdwg::graph<std::string, int>;
	const auto vector_il = std::vector<std::string>{"Hey", "hello", "why", "??", "lol", "Kappa123", "PauseChamp"};
	const auto g = graph(vector_il.begin(), vector_il.end());
	const auto all_nodes = g.nodes();
	auto sorted_nodes = vector_il;
	std::sort(sorted_nodes.begin(), sorted_nodes.end());
	CHECK(!g.empty());
	CHECK(std::is_sorted(all_nodes.begin(), all_nodes.end()));
	CHECK(std::equal(all_nodes.begin(), all_nodes.end(), sorted_nodes.begin()));
}