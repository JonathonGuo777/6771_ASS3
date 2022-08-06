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
//	CHECK(g.find(1, 1, 50) == g.end());
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
	auto g = gdwg::graph<int, int>{1, 2, 3};


	SECTION("edge part") {
		CHECK(g.insert_edge(1, 2, 100));
		CHECK(g.insert_edge(2, 1, 200));
		CHECK(g.insert_edge(1, 1, 50));
		CHECK(g.insert_edge(1, 1, 200));

		CHECK(g.replace_node(1, 99));

		// check delete edges
		CHECK(g.find(1, 2, 100) == g.end());
		CHECK(g.find(2, 1, 200) == g.end());
		CHECK(g.find(1, 1, 50) == g.end());
		CHECK(g.find(1, 1, 200) == g.end());


		CHECK(g.is_node(99));
		CHECK_FALSE(g.is_node(1));

		// check edges
		CHECK(g.find(99, 2, 100) != g.end());
		CHECK(g.find(2, 99, 200) != g.end());
		CHECK(g.find(99, 99, 50) != g.end());
		CHECK(g.find(99, 99, 200) != g.end());
	}

	SECTION("replace succedd"){
		CHECK(g.replace_node(1, 99));

		CHECK(g.is_node(99));
		CHECK(!g.is_node(1));
	}

	// replace node already exist
	SECTION("replace exist node"){
		CHECK_FALSE(g.replace_node(1, 2));

		CHECK(g.is_node(1));
		CHECK(g.is_node(2));
	}

	// no src, with dst
	CHECK_THROWS(g.replace_node(66, 2));
	// no src no dst
	CHECK_THROWS(g.replace_node(44, 55));

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




