#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

// insert node
TEST_CASE("insert_node_no_duplicate") {
	using graph = gdwg::graph<int, int>;
	auto g = graph();
	CHECK(g.empty());
	auto inserted = g.insert_node(1);
	CHECK(g.nodes().size() == 1);
	CHECK(g.nodes().at(0) == 1);
	CHECK(inserted);
}

TEST_CASE("insert_node_with_duplicate") {
	using graph = gdwg::graph<int, int>;
	auto g = graph();
	CHECK(g.empty());
	auto inserted = g.insert_node(1);
	SECTION("check initial insert is okay") {
		CHECK(g.nodes().size() == 1);
		CHECK(g.nodes().at(0) == 1);
		CHECK(inserted);
	}
	SECTION("Check duplicate insert has no effect") {
		inserted = g.insert_node(1);
		CHECK(g.nodes().size() == 1);
		CHECK(g.nodes().at(0) == 1);
		CHECK(!inserted);
	}
}

// insert edge
TEST_CASE("insert_edge_default") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	auto inserted = g.insert_edge(1, 2, 900);
	CHECK(inserted);
	inserted = g.insert_edge(1, 2, 4);
	CHECK(inserted);
	inserted = g.insert_edge(1, 2, 4);
	CHECK(!inserted);
	inserted = g.insert_edge(1, 3, 5);
	CHECK(inserted);
	CHECK(g.connections(1) == std::vector<int>{2, 3});
	// checks weights sorted
	CHECK(g.weights(1, 2) == std::vector<int>{4, 900});
}

TEST_CASE("insert_edge_missing_node") {
	using graph = gdwg::graph<int, int>;
	auto default_graph = graph();
	auto g = graph{1, 2, 3, 4};
	CHECK_THROWS_WITH(g.insert_edge(1, 69, 900),
	                  "Cannot call gdwg::graph<N, E>::insert_edge when either from or to node does not exist");
	CHECK_THROWS_WITH(g.insert_edge(69, 1, 900),
	                  "Cannot call gdwg::graph<N, E>::insert_edge when either from or to node does not exist");
	CHECK_THROWS_WITH(g.insert_edge(69, 69, 900),
	                  "Cannot call gdwg::graph<N, E>::insert_edge when either from or to node does not exist");
}

// erase node
TEST_CASE("remove_node_default") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	g.insert_edge(1, 2, 9);
	CHECK(g.erase_node(1));
	CHECK(g.nodes() == std::vector<int>{2, 3, 4});
	CHECK(g.connections(2) == std::vector<int>{});
	CHECK(g.connections(3) == std::vector<int>{});
	CHECK(g.connections(4) == std::vector<int>{});
}

TEST_CASE("remove_node_false_cases") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	g.insert_edge(1, 2, 9);
	CHECK(!g.erase_node(69));
	CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4});
	CHECK(g.connections(1) == std::vector<int>{2});
	CHECK(g.weights(1, 2) == std::vector<int>{9});
	CHECK(g.connections(2) == std::vector<int>{});
	CHECK(g.connections(3) == std::vector<int>{});
	CHECK(g.connections(4) == std::vector<int>{});
}

// erase edge
TEST_CASE("remove_edges_default") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	g.insert_edge(1, 2, 9);
	SECTION("Checking graph before erase conditions") {
		CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4});
		CHECK(g.connections(1) == std::vector<int>{2});
		CHECK(g.weights(1, 2) == std::vector<int>{9});
		CHECK(g.connections(2) == std::vector<int>{});
		CHECK(g.connections(3) == std::vector<int>{});
		CHECK(g.connections(4) == std::vector<int>{});
	}
	SECTION("Removing the edge and checking graph post conditions"){
		CHECK(g.erase_edge(1, 2, 9));
		CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4});
		CHECK(g.connections(1) == std::vector<int>{});
		CHECK(g.weights(1, 2) == std::vector<int>{});
		CHECK(g.connections(2) == std::vector<int>{});
		CHECK(g.connections(3) == std::vector<int>{});
		CHECK(g.connections(4) == std::vector<int>{});
	}
}

TEST_CASE("remove_edges_errors") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	g.insert_edge(1, 2, 9);
	SECTION("Check graph is made properly") {
		CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4});
		CHECK(g.connections(1) == std::vector<int>{2});
		CHECK(g.weights(1, 2) == std::vector<int>{9});
		CHECK(g.connections(2) == std::vector<int>{});
		CHECK(g.connections(3) == std::vector<int>{});
		CHECK(g.connections(4) == std::vector<int>{});
	}
	SECTION("erase edge returns false for nodes that exist") {
		CHECK(!g.erase_edge(1, 2, 99));
		CHECK(!g.erase_edge(1, 3, 99));
		CHECK(!g.erase_edge(2, 2, 99));
		CHECK(!g.erase_edge(2, 2, 9));
	}
	SECTION("erase edge throws exception when nodes either from and to dont exist") {
		CHECK_THROWS_WITH(g.erase_edge(2, 29, 9),
		                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
		CHECK_THROWS_WITH(g.erase_edge(29, 2, 9),
		                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
		CHECK_THROWS_WITH(g.erase_edge(82, 29, 9),
		                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
	}
	SECTION("Check graph is unmodified from these fail cases above") {
		CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4});
		CHECK(g.connections(1) == std::vector<int>{2});
		CHECK(g.weights(1, 2) == std::vector<int>{9});
		CHECK(g.connections(2) == std::vector<int>{});
		CHECK(g.connections(3) == std::vector<int>{});
		CHECK(g.connections(4) == std::vector<int>{});
	}
}

// erase edge and iterator
TEST_CASE("default_delete_iterator") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	CHECK(g.insert_edge(1, 2, 3));
	CHECK(g.weights(1, 2) == std::vector<int>{3});
	auto iterator_begin = g.begin();
	CHECK(*iterator_begin == gdwg::graph<int, int>::value_type{1, 2, 3});
	auto next_iterator = g.erase_edge(iterator_begin);
	CHECK(next_iterator == g.end());
	CHECK(g.weights(1, 2) == std::vector<int>{});
}

TEST_CASE("multiple_iterator_erase_few") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	CHECK(g.insert_edge(1, 2, 3));
	CHECK(g.insert_edge(1, 3, 3));
	CHECK(g.insert_edge(1, 4, 3));
	CHECK(g.weights(1, 2) == std::vector<int>{3});
	auto iterator_begin = g.begin();
	auto iterator_end = iterator_begin++;
	auto next_iterator = g.erase_edge(iterator_begin, iterator_end);
	CHECK(*next_iterator == gdwg::graph<int, int>::value_type{1, 3, 3});
	// check graph is reset
	CHECK(*g.begin() == gdwg::graph<int, int>::value_type{1, 3, 3});
}

TEST_CASE("multiple_iterator_erase_all") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{1, 2, 3, 4};
	CHECK(g.insert_edge(1, 2, 3));
	CHECK(g.insert_edge(1, 3, 3));
	CHECK(g.insert_edge(1, 4, 3));
	CHECK(g.weights(1, 2) == std::vector<int>{3});
	auto iterator_begin = g.begin();
	auto iterator_end = g.end();
	auto next_iterator = g.erase_edge(iterator_begin, iterator_end);
	CHECK(next_iterator == g.end());
	// check graph is reset
	CHECK(g.begin() == g.end());
	CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4});
}

// replace node
TEST_CASE("replace_node_success") {
	using graph = gdwg::graph<std::string, int>;
	auto g = graph{"hello", "replace me"};
	g.insert_edge("hello", "replace me", 5);
	auto replaced = g.replace_node("hello", "replaced_hello");
	CHECK(replaced);
	auto is_connected = g.is_connected("replaced_hello", "replace me");
	CHECK(is_connected);
	replaced = g.replace_node("replace me", "replaced");
	CHECK(replaced);
	is_connected = g.is_connected("replaced_hello", "replaced");
	CHECK(is_connected);
	CHECK(g.nodes() == std::vector<std::string>{"replaced", "replaced_hello"});
	CHECK(g.connections("replaced_hello") == std::vector<std::string>{"replaced"});
	CHECK(g.weights("replaced_hello", "replaced") == std::vector<int>{5});
}

TEST_CASE("replace_node_errors") {
	using graph = gdwg::graph<std::string, int>;
	auto g = graph{"hello", "replace me"};
	CHECK_THROWS_WITH(g.replace_node("helloWRONG", "replaced"), "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
}

TEST_CASE("replace_node_fail") {
	using graph = gdwg::graph<std::string, int>;
	auto g = graph{"hello", "replace me"};
	auto replaced = g.replace_node("hello", "replace me");
	CHECK(!replaced);
	 CHECK(

// merge replace node
TEST_CASE("merge_replace_all_covered") {
		using graph = gdwg::graph<std::string, int>;
		auto g = graph{"A", "B", "C", "D"};
		g.insert_edge("A", "B", 1);
		g.insert_edge("A", "A", 1);
		g.insert_edge("A", "A", 5);
		g.insert_edge("A", "C", 2);
		g.insert_edge("C", "A", 9);
		g.insert_edge("A", "D", 3);
		g.insert_edge("B", "B", 9);
		g.merge_replace_node("A", "B");
		SECTION("Check the graph state after merge replace") {
			CHECK(g.nodes() == std::vector<std::string>{"B", "C", "D"});
			CHECK(g.connections("B") == std::vector<std::string>{"B", "C", "D"});
			CHECK(g.connections("C") == std::vector<std::string>{"B"});
			CHECK(g.weights("B", "B") == std::vector<int>{1, 5, 9});
			CHECK(g.weights("B", "C") == std::vector<int>{2});
			CHECK(g.weights("B", "D") == std::vector<int>{3});
			CHECK(g.weights("C", "B") == std::vector<int>{9});
		}
}

TEST_CASE("merge_replace_same") {
		using graph = gdwg::graph<std::string, int>;
		auto g = graph{"A", "B", "C", "D"};
		g.insert_edge("A", "B", 1);
		g.insert_edge("A", "C", 2);
		g.insert_edge("A", "D", 3);
		g.insert_edge("B", "B", 1);
		SECTION("Check graph before merge replace is called") {
			CHECK(g.nodes() == std::vector<std::string>{"A", "B", "C", "D"});
			CHECK(g.connections("A") == std::vector<std::string>{"B", "C", "D"});
			CHECK(g.weights("B", "B") == std::vector<int>{1});
			CHECK(g.weights("B", "C") == std::vector<int>{});
			CHECK(g.weights("B", "D") == std::vector<int>{});
			CHECK(g.weights("A", "B") == std::vector<int>{1});
			CHECK(g.weights("A", "C") == std::vector<int>{2});
			CHECK(g.weights("A", "D") == std::vector<int>{3});
		}
		g.merge_replace_node("A", "A");
		SECTION("Check same state of graph after merge replacing with the same node") {
			CHECK(g.nodes() == std::vector<std::string>{"A", "B", "C", "D"});
			CHECK(g.connections("A") == std::vector<std::string>{"B", "C", "D"});
			CHECK(g.weights("B", "B") == std::vector<int>{1});
			CHECK(g.weights("B", "C") == std::vector<int>{});
			CHECK(g.weights("B", "D") == std::vector<int>{});
			CHECK(g.weights("A", "B") == std::vector<int>{1});
			CHECK(g.weights("A", "C") == std::vector<int>{2});
			CHECK(g.weights("A", "D") == std::vector<int>{3});
		}
}

TEST_CASE("merge_replace_errors") {
		using graph = gdwg::graph<std::string, int>;
		auto g = graph{"A", "B", "C", "D"};
		g.insert_edge("A", "B", 1);
		g.insert_edge("A", "C", 2);
		g.insert_edge("A", "D", 3);
		g.insert_edge("B", "B", 1);
		CHECK_THROWS_WITH(g.merge_replace_node("A", "F"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
		CHECK_THROWS_WITH(g.merge_replace_node("G", "F"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
		CHECK_THROWS_WITH(g.merge_replace_node("B", "F"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
}

// clear
TEST_CASE("clear_graphs") {
		using graph = gdwg::graph<int, int>;
		auto g = graph{1,2,3,4,5,6,7,8,0, 9};
		CHECK(g.nodes() == std::vector<int>{0,1,2,3,4,5,6,7,8,9});
		g.clear();
		CHECK(g.empty());
}

