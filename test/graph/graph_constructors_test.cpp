#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Default") {
	auto g = gdwg::graph<int, std::string>{};
	CHECK(g.empty());
}

TEST_CASE("Initializer list") {
	SECTION("int") {
		auto g = gdwg::graph<int, std::string>{-3, 19, 20};
		CHECK(!g.empty());
		CHECK(g.is_node(-3));
		CHECK(g.is_node(19));
		CHECK(g.is_node(20));
		// error
		CHECK(!g.is_node(0));
		CHECK(!g.is_node(5));
	}
	SECTION("string") {
		auto g = gdwg::graph<std::string, std::string>{"a", "b", "text"};
		CHECK(!g.empty());
		CHECK(g.is_node("a"));
		CHECK(g.is_node("b"));
		CHECK(g.is_node("text"));
		// error
		CHECK(!g.is_node(""));
		CHECK(!g.is_node("c"));
	}
	SECTION("std::vector") {
		auto v1 = std::vector<int>{1, 2, 3};
		auto v2 = std::vector<int>{-1, -2, 0};
		auto v3 = std::vector<int>{4, 5, 6};
		auto v4 = std::vector<int>{7, 8, 9};
		auto g = gdwg::graph<std::vector<int>, std::string>{v1, v2, v3};
		CHECK(!g.empty());
		CHECK(g.is_node(std::vector<int>(v1)));
		CHECK(g.is_node(std::vector<int>(v2)));
		CHECK(g.is_node(std::vector<int>(v3)));
		// error
		CHECK(!g.is_node(std::vector<int>(v4)));
	}
}


TEST_CASE("InputIt") {
	SECTION("int vector") {
		auto v = std::vector<int>{11, 99};
		auto g = gdwg::graph<int, std::string>(v.begin(), v.end());

		CHECK(!g.empty());
		CHECK(g.is_node(11));
		CHECK(g.is_node(99));
		//error
		CHECK(!g.is_node(22));
		CHECK(!g.is_node(-2));
	}

	SECTION("string set") {
		auto s = std::set<std::string>{"a", "b", "text"};
		auto g = gdwg::graph<std::string, int>(s.begin(), s.end());
		CHECK(!g.empty());
		CHECK(g.is_node("a"));
		CHECK(g.is_node("b"));
		CHECK(g.is_node("text"));
		// error
		CHECK(!g.is_node(""));
		CHECK(!g.is_node("c"));
	}
}


TEST_CASE("Copy Constructor") {

	SECTION("nodes") {
		auto const g = gdwg::graph<std::string, int>{"a", "b", "text"};
		auto g_copy = g;
		// check same nodes
		CHECK(!g.empty());
		CHECK(g.is_node("a"));
		CHECK(g.is_node("b"));
		CHECK(g.is_node("text"));
		// error
		CHECK(!g.is_node(""));
		CHECK(!g.is_node("c"));

		CHECK(!g_copy.empty());
		CHECK(g_copy.is_node("a"));
		CHECK(g_copy.is_node("b"));
		CHECK(g_copy.is_node("text"));
		// error
		CHECK(!g_copy.is_node(""));
		CHECK(!g_copy.is_node("c"));

	}
	SECTION("Edges") {
		auto g = gdwg::graph<std::string, int>{"a", "b", "text"};
		g.insert_edge("a", "b", 4);
		g.insert_edge("b", "text", 8);

		auto g_copy = g;

		// Check edges by iterator
		auto g_copy_it = g_copy.begin();
		CHECK((*g_copy_it).from == "a");
		CHECK((*g_copy_it).to == "b");
		CHECK((*g_copy_it).weight == 4);

		++g_copy_it;
		CHECK((*g_copy_it).from == "b");
		CHECK((*g_copy_it).to == "text");
		CHECK((*g_copy_it).weight == 8);
	}
}

TEST_CASE("Copy Assignment") {
	SECTION("nodes") {
		auto const g = gdwg::graph<std::string, int>{"a", "b", "text"};
		auto g_copy = gdwg::graph<std::string, int>{"c", "d"};

		CHECK(g.is_node("a"));
		CHECK(g.is_node("b"));
		CHECK(g.is_node("text"));

		CHECK(g_copy.is_node("c"));
		CHECK(g_copy.is_node("d"));

		g_copy = g;

		CHECK(g.is_node("a"));
		CHECK(g.is_node("b"));
		CHECK(g.is_node("text"));

		CHECK(g_copy.is_node("a"));
		CHECK(g_copy.is_node("b"));
		CHECK(g_copy.is_node("text"));
		//before
		CHECK(!g_copy.is_node("c"));
		CHECK(!g_copy.is_node("d"));
	}
	SECTION("Edges") {
		auto g = gdwg::graph<std::string, int>{"a", "b", "text"};
		g.insert_edge("a", "b", 4);
		g.insert_edge("b", "text", 8);

		auto g_copy = gdwg::graph<std::string, int>();
		g_copy = g;

		// Check edges by iterator
		auto g_copy_it = g_copy.begin();
		CHECK((*g_copy_it).from == "a");
		CHECK((*g_copy_it).to == "b");
		CHECK((*g_copy_it).weight == 4);

		++g_copy_it;
		CHECK((*g_copy_it).from == "b");
		CHECK((*g_copy_it).to == "text");
		CHECK((*g_copy_it).weight == 8);
	}
}

TEST_CASE("Move Constructor") {

	SECTION("Check moved dst contents and the moved src is emtpy") {
		auto moved_src = gdwg::graph<std::string, int>{"a", "b", "text"};
		auto moved_dst = gdwg::graph<std::string, int>(std::move(moved_src));

		CHECK(moved_dst.is_node("a"));
		CHECK(moved_dst.is_node("b"));
		CHECK(moved_dst.is_node("text"));

		CHECK(moved_src.empty());
	}

	SECTION("Check iterator") {
		auto moved_src = gdwg::graph<std::string, int>{"a", "b", "text"};
		auto moved_dst = gdwg::graph<std::string, int>{"b", "c"};
		moved_src.insert_edge("a", "b", 4);
		moved_dst.insert_edge("b", "c", 5);

		auto moved_src_it = moved_src.begin();
		auto moved_dst_it = moved_dst.begin();

		// before move, check iterator
		CHECK((*moved_dst_it).from == "b");
		CHECK((*moved_dst_it).to == "c");
		CHECK((*moved_dst_it).weight == 5);

		CHECK((*moved_src_it).from == "a");
		CHECK((*moved_src_it).to == "b");
		CHECK((*moved_src_it).weight == 4);

		// move
		moved_dst = gdwg::graph<std::string, int>(std::move(moved_src));

		// after move
		// still valid
		CHECK((*moved_src_it).from == "a");
		CHECK((*moved_src_it).to == "b");
		CHECK((*moved_src_it).weight == 4);

	}
}

TEST_CASE("Move Assignment") {
	auto moved_src = gdwg::graph<std::string, int>{"a", "b", "text"};
	auto moved_dst = gdwg::graph<std::string, int>();

	SECTION("Check moved dst contents and the moved src is emtpy") {
		moved_dst = std::move(moved_src);

		CHECK(moved_dst.is_node("a"));
		CHECK(moved_dst.is_node("b"));
		CHECK(moved_dst.is_node("text"));

		CHECK(moved_src.empty());
	}


	SECTION("Check iterator") {
		moved_src.insert_edge("a", "b", 5);
		auto moved_from_it = moved_src.begin();

		moved_dst = std::move(moved_src);
		CHECK((*moved_from_it).from == "a");
		CHECK((*moved_from_it).to == "b");
		CHECK((*moved_from_it).weight == 5);
	}

}