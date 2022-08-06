#include "gdwg/graph.hpp"
#include <catch2/catch.hpp>
#include <vector>

TEST_CASE("Is node") {
	auto g = gdwg::graph<int, std::string>{1,2,3};

	CHECK(g.is_node(1));
	CHECK(!g.is_node(10));
}

TEST_CASE("Empty") {
	auto g1 = gdwg::graph<int, std::string>();
	auto g2 = gdwg::graph<int, std::string>{1};

	CHECK(g1.empty());
	CHECK(!g2.empty());
}

TEST_CASE("Is connected") {
	auto g1 = gdwg::graph<int, int>{1, 2, 3};

	g1.insert_edge(2, 1, 22);
	g1.insert_edge(1, 2, 33);
	g1.insert_edge(2, 3, 44);

	CHECK(g1.is_connected(1, 2));
	CHECK(!g1.is_connected(1, 1));

	// no src or dst
	CHECK_THROWS(g1.is_connected(99, 2));
	CHECK_THROWS(g1.is_connected(1, 99));
}

TEST_CASE("Nodes") {
	SECTION("list init ") {
		auto v = std::vector<int>{0, 1, 2, 3};
		auto g = gdwg::graph<int, std::string>{1, 2, 3, 0};
		CHECK(v == g.nodes());
		CHECK(g.nodes() == g.nodes());
	}

	SECTION("vector init") {
		auto v1 = std::vector<int>{0, 1, 2, 3, 777};
		auto g1 = gdwg::graph<int, std::string>(v1.begin(), v1.end());
		CHECK(g1.nodes() == v1);
	}
}

TEST_CASE("Weights") {
	auto g1 = gdwg::graph<int, int>{1, 2, 3};

	g1.insert_edge(1, 2, 6);
	g1.insert_edge(1, 2, 5);
	g1.insert_edge(2, 1, 8);
	g1.insert_edge(2, 1, 7);

	g1.insert_edge(2, 2, 9);
	g1.insert_edge(2, 2, 10);

	CHECK(g1.weights(1, 2) == std::vector<int>{5, 6});
	CHECK(g1.weights(2, 1) == std::vector<int>{7, 8});
	CHECK(g1.weights(2, 2) == std::vector<int>{9, 10});
}

TEST_CASE("Find") {
	auto g1 = gdwg::graph<int, int>{1, 2, 3};

	g1.insert_edge(2, 1, 7);
	g1.insert_edge(1, 2, 6);
	g1.insert_edge(1, 2, 5);
	g1.insert_edge(2, 1, 8);

	CHECK(g1.find(1, 2, 5) == g1.begin());
	CHECK(g1.find(2, 3, 6) == g1.end());
	CHECK(g1.find(22, 33, 44) == g1.end());
}

TEST_CASE("Connection") {
	auto g1 = gdwg::graph<int, int>{1, 2, 3};

	g1.insert_edge(2, 1, 7);
	g1.insert_edge(1, 2, 6);
	g1.insert_edge(1, 2, 5);
	g1.insert_edge(2, 1, 8);

	g1.insert_edge(2, 2, 9);
	g1.insert_edge(2, 2, 10);

	CHECK(g1.connections(2) == std::vector<int>{1,1,2,2});
	CHECK(g1.connections(1) == std::vector<int>{2,2});

	CHECK_THROWS(g1.connections(99));
}