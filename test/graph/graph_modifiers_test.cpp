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

TEST_CASE("Merge replace node") {
	SECTION("simple edge") {
		auto g1 = gdwg::graph<int, int>{1, 2, 3};

		g1.insert_edge(2, 1, 7);
		g1.insert_edge(1, 2, 6);
		g1.insert_edge(1, 2, 5);
		g1.insert_edge(2, 1, 8);


		g1.merge_replace_node(1, 2);

		CHECK(g1.weights(2, 2) == std::vector<int>{5, 6, 7, 8});

		g1.merge_replace_node(2, 3);

		CHECK(g1.weights(3, 3) == std::vector<int>{5, 6, 7, 8});

		// no src, no dst, or both
		CHECK_THROWS(g1.merge_replace_node(2, 99));
		CHECK_THROWS(g1.merge_replace_node(99, 1));
		CHECK_THROWS(g1.merge_replace_node(55, 66));
	}

	SECTION("With reflexive edge") {
		auto g2 = gdwg::graph<int, int>{1, 2, 3};
		g2.insert_edge(1, 1, 7);
		g2.insert_edge(2, 2, 7);

		g2.merge_replace_node(1, 2);

		CHECK(g2.weights(2, 2) == std::vector<int>{7});
	}

}


//TEST_CASE("Merge and replace node") {
//	auto g = gdwg::graph<std::string, int>{"Yoona", "Taeyeon", "Tzuyu"};
//
//	SECTION("Simple case: No duplicate edge after replacement") {
//		g.insert_edge("Yoona", "Taeyeon", 309);
//		g.insert_edge("Yoona", "Yoona", 530);
//
//		g.merge_replace_node("Yoona", "Tzuyu");
//
//		// Check old edge are removed
//		CHECK(g.find("Yoona", "Taeyeon", 309) == g.end());
//		CHECK(g.find("Yoona", "Yoona", 530) == g.end());
//
//		// Check the existence of the new edges
//		CHECK(g.find("Tzuyu", "Tzuyu", 530) != g.end());
//		CHECK(g.find("Tzuyu", "Taeyeon", 309) != g.end());
//
//		// Check that the old node is remove
//		CHECK_FALSE(g.is_node("Yoona"));
//	}
//
//	SECTION("Hard case: Edges need to be merged") {
//		g.insert_edge("Yoona", "Tzuyu", 309);
//		g.insert_edge("Tzuyu", "Tzuyu", 309);
//		g.insert_edge("Yoona", "Taeyeon", 520);
//		g.insert_edge("Tzuyu", "Taeyeon", 520);
//
//		g.merge_replace_node("Yoona", "Tzuyu");
//
//		// Check that the old node is remove
//		CHECK_FALSE(g.is_node("Yoona"));
//
//		// Check that edges have been merged, by checking the printed graph
//		auto oss = std::ostringstream();
//		oss << g;
//		auto const expected_oss = std::string_view(R"(Taeyeon (
//		)
//		Tzuyu (
//		  Taeyeon | 520
//		  Tzuyu | 309
//		)
//		)");
//		CHECK(oss.str() == expected_oss);
//	}
//
//	SECTION("Exception: either is_node(old_data) or is_node(new_data) are false") {
//		// src not exist, dst exist
//		CHECK_THROWS_MATCHES(g.merge_replace_node("Yeonwoo", "Taeyeon"),
//		                     std::runtime_error,
//		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
//		                                              "E>::merge_replace_node on old or new data if "
//		                                              "they don't exist in the graph"));
//
//		// dst not exist, src exist
//		CHECK_THROWS_MATCHES(g.merge_replace_node("Taeyeon", "Yeonwoo"),
//		                     std::runtime_error,
//		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
//		                                              "E>::merge_replace_node on old or new data if "
//		                                              "they don't exist in the graph"));
//
//		// Both not exist
//		CHECK_THROWS_MATCHES(g.merge_replace_node("Yeonwoo", "Mina"),
//		                     std::runtime_error,
//		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
//		                                              "E>::merge_replace_node on old or new data if "
//		                                              "they don't exist in the graph"));
//	}
//}


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




