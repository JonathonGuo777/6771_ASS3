#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

TEST_CASE("Insert node") {
	auto g = gdwg::graph<int, std::string>{6};
	CHECK(g.insert_node(7));
	CHECK(g.insert_node(8));

	CHECK(g.is_node(6));
	CHECK(g.is_node(7));
	CHECK(g.is_node(8));
	// insert existing node
	CHECK(!g.insert_node(6));
	CHECK(!g.insert_node(7));
}

TEST_CASE("Insert edge") {
	auto g1 = gdwg::graph<int, int>{1, 2, 3};
	CHECK(g1.insert_edge(1, 2, 50));
	CHECK(g1.insert_edge(2, 3, 100));
	// insert existing edge
	CHECK(!g1.insert_edge(2, 3, 100));
	CHECK_THROWS(g1.insert_edge(2, 99, 7));
	CHECK_THROWS(g1.insert_edge(99, 1, 7));
	CHECK_THROWS(g1.insert_edge(4, 5, 7));
}

TEST_CASE("Clear"){
	      // nodes
	      SECTION("Clear nodes") {
		      auto g = gdwg::graph<int, int>{1, 2, 3};
		      g.clear();
		      CHECK(g.empty());
	      }
	      // edges
	      SECTION("Clear edges") {
		      auto g = gdwg::graph<int, int>{1, 2, 3};
		      g.insert_edge(1, 2, 50);
		      g.insert_edge(2, 3, 100);
		      g.clear();
		      CHECK(g.empty());
	      }
}
//
//	// Non-relevant is not removed
//	CHECK(g.find(2, 3, 309) != g.end());
//
//	// Relevant ones are removed
//	CHECK(g.find(1, 1, 530) == g.end());
//	CHECK(g.find(1, 1, 520) == g.end());
//	CHECK(g.find(1, 3, 309) == g.end());}

//TEST_CASE("Erase node"){
//	auto g = gdwg::graph<int, int>{1, 2, 3, 4};
//	g.insert_edge(1, 2, 50);
//	g.insert_edge(2, 3, 100);
//	g.insert_edge(3, 4, 200);
//
//	CHECK(g.erase_node(1));
//	CHECK(!g.erase_node(1));
//
//	// Only the particular edge is removed
//	CHECK(g.find(3, 4, 200) == g.end());
//	CHECK(g.find(2, 3, 100) != g.end());
//
//}

TEST_CASE("Erase node") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	g.insert_edge(1, 2, 50);
	g.insert_edge(2, 3, 100);
	// erase non-existing node
	CHECK(!g.erase_node(4));
	// erase existing node
	CHECK(g.erase_node(1));
	CHECK(!g.erase_node(1));

	// remove relevant edges
	CHECK(g.find(1, 2, 50) == g.end());

	// keep non-relevant edges
	CHECK(g.find(2, 3, 100) != g.end());

}


TEST_CASE("Erase edge (src, dst, weight)") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	g.insert_edge(1, 2, 50);
	g.insert_edge(2, 3, 100);
	// erase non-existing edge
	CHECK(!g.erase_edge(1, 2, 200));
	// erase existing edge
	CHECK(g.erase_edge(1, 2, 50));

	// remove target edges
	CHECK(g.find(1, 2, 50) == g.end());

	// keep non-relevant edges
	CHECK(g.find(2, 3, 100) != g.end());

	// throw exception if edge src, dst false
	CHECK_THROWS(g.erase_edge(2, 4, 50));
	CHECK_THROWS(g.erase_edge(4, 3, 50));
	CHECK_THROWS(g.erase_edge(7, 8, 50));
}


TEST_CASE("Erase edge: (iterator i)") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	
	// remove non-existing edge
	CHECK(g.erase_edge(g.begin()) == g.end());
	CHECK(g.erase_edge(g.end()) == g.end());
	CHECK(g.erase_edge(gdwg::graph<int, int>::iterator()) == g.end());
	
	
	g.insert_edge(3, 2, 2);
	g.insert_edge(1, 2, 7);
	g.insert_edge(3, 2, 4);
	

	CHECK(g.erase_edge(g.find(3, 2, 2)) == g.find(3, 2, 4));

	// Only target edge is removed
	CHECK(g.find(3, 2, 2) == g.end());
	CHECK(g.find(1, 2, 7) != g.end());
	CHECK(g.find(3, 2, 4) != g.end());
	
}
TEST_CASE("Erase edge: (iterator i, iterator s)") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	
	g.insert_edge(3, 2, 2);
	g.insert_edge(1, 2, 17);
	g.insert_edge(3, 2, 4);

	CHECK(g.erase_edge(g.find(3, 2, 2), g.find(3, 2, 4))
	      == g.find(3, 2, 4));

	// Only target edge is removed
	CHECK(g.find(3, 2, 2) == g.end());
	CHECK(g.find(1, 2, 17) != g.end());
	CHECK(g.find(3, 2, 4) != g.end());
}


TEST_CASE("Replace Node") {
	auto g = gdwg::graph<std::string, int>{"Yoona", "Taeyeon", "Tzuyu"};

	SECTION("Simple case: If node are replaced") {
		CHECK(g.replace_node("Yoona", "Rose"));

		CHECK(g.is_node("Rose"));
		CHECK_FALSE(g.is_node("Yoona"));
	}

	SECTION("Hard case: If edges are also replaced") {
		CHECK(g.insert_edge("Yoona", "Taeyeon", 818));
		CHECK(g.insert_edge("Taeyeon", "Yoona", 1314));
		CHECK(g.insert_edge("Yoona", "Yoona", 530));
		CHECK(g.insert_edge("Yoona", "Yoona", 1314));

		CHECK(g.replace_node("Yoona", "Rose"));

		// Check that the edges no longer exist
		CHECK(g.find("Yoona", "Taeyeon", 818) == g.end());
		CHECK(g.find("Taeyeon", "Yoona", 1314) == g.end());
		CHECK(g.find("Yoona", "Yoona", 530) == g.end());
		CHECK(g.find("Yoona", "Yoona", 1314) == g.end());

		// Check that the old node doesn't exist, and new node exist
		CHECK(g.is_node("Rose"));
		CHECK_FALSE(g.is_node("Yoona"));

		// Check we have these edges
		CHECK(g.find("Rose", "Taeyeon", 818) != g.end());
		CHECK(g.find("Taeyeon", "Rose", 1314) != g.end());
		CHECK(g.find("Rose", "Rose", 530) != g.end());
		CHECK(g.find("Rose", "Rose", 1314) != g.end());
	}

	SECTION("new_data already exist") {
		CHECK_FALSE(g.replace_node("Yoona", "Taeyeon"));

		CHECK(g.is_node("Yoona"));
		CHECK(g.is_node("Taeyeon"));
	}

	SECTION("Exception: is_node(old_data) == false") {
		// src, dst not exist
		CHECK_THROWS_MATCHES(g.replace_node("Yeonwoo", "Mina"),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node "
		                                              "on a node that doesn't exist"));

		// src not exist, dst exist
		CHECK_THROWS_MATCHES(g.replace_node("Yeonwoo", "Taeyeon"),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node "
		                                              "on a node that doesn't exist"));
	}
}



TEST_CASE("Merge replace node"){

}


//TEST_CASE("Erase edge (iterator i)") {
//	auto g = gdwg::graph<int, int>{1, 2, 3};
//	g.insert_edge(1, 2, 50);
//	g.insert_edge(2, 3, 100);
//
//	// erase non-existing edge
//	CHECK(g.erase_edge(g.begin()) == g.end());
//	CHECK(g.erase_edge(g.end()) == g.end());
//
//	// erase existing edge
//	CHECK(g.erase_edge(g.find(1, 2, 50)) == g.find(1, 2, 50));
//
//	// remove target edges
//	CHECK(g.find(1, 2, 50) == g.end());
//	// keep non-relevant edges
//	CHECK(g.find(2, 3, 100) != g.end());
//
//}




