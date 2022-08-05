#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>


// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph{
	public:
		struct value_type {
			N from;
			N to;
			E weight;
		};

		struct edge {
			std::shared_ptr<N> src;
			std::shared_ptr<N> dst;
			E weight;
		};
		//		struct edge {
		//			N* src;
		//			N* dst;
		//			E weight;
		//		};

		// Constructor
		graph() noexcept = default;

		graph(std::initializer_list<N> il){
			*this = graph(il.begin(), il.end());
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last){
			for (auto& it = first; it != last; ++it) {
				nodes_.emplace(std::make_shared<N>(*it));
			}
		}

		graph(graph&& other) noexcept
		: nodes_{std::exchange(other.nodes_, std::set<std::shared_ptr<N>, node_cmp>())}
		, edges_{std::exchange(other.edges_, std::set<std::shared_ptr<edge>, edge_cmp>())} {}

		// to be edited
		//		auto operator=(graph&& other) noexpect -> graph&{
		//			std::swap(nodes_, other.nodes_);
		//			std::swap(nodes_, other.nodes_);
		//			return *this;
		//		}
		auto operator=(graph&& other) noexcept -> graph& {
			std::swap(nodes_, other.nodes_);
			std::swap(nodes_, other.nodes_);
			return *this;
		}

		// to be tested
		graph(graph const& other){
			for (auto& it : other.nodes_) {
				nodes_.emplace(std::make_shared<N>(*it));
			}
			for (auto& it : other.edges_) {
				edges_.emplace(std::make_shared<edge>(*it));
			}
		}

		auto operator=(graph const& other) -> graph&{
			if (this == &other) { // to edit
				return *this;
			}
			auto obj = graph(other);
			swap(*this, obj);
			return *this;
		}


		//		// Modifiers
		auto insert_node(N const& value) -> bool{
			return nodes_.emplace(std::make_shared<N>(value)).second;
		}
		//
		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool{
			if (is_node(src) and is_node(dst)) {
				return edges_.emplace(
				                std::make_shared<edge>(
				                   edge{*nodes_.find(src), *nodes_.find(dst), weight})).second;

			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
			                         "or dst node does not exist");
		}
		//
		//		auto replace_node(N const& old_data, N const& new_data) -> bool{
		//			if (!is_node(old_data)) {
		//				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
		//				                         "doesn't exist");
		//			}
		//			if (is_node(new_data)) {
		//				return false;
		//			}
		//
		//		}
		// to be edited
		auto replace_node(N const& old_data, N const& new_data) -> bool {
			auto old_it = nodes_.find(old_data);
			if (old_it == std::end(nodes_)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}

			if (is_node(new_data)) {
				return false;
			}

			// Insert the new node
			auto new_node = std::make_shared<N>(new_data);
			nodes_.emplace(new_node);

			// Find all relevant edges
			auto edge_ptrs = std::vector<std::shared_ptr<edge>>();
			std::copy_if(edges_.begin(),
			             edges_.end(),
			             std::back_inserter(edge_ptrs),
			             [&](auto const& e_ptr) {
				             return *(e_ptr->src) == old_data or *(e_ptr->dst) == old_data;
			             });

			// Replace the nodes
			for (auto const& e_ptr : edge_ptrs) {
				auto new_src_ptr = *(e_ptr->src) == old_data ? new_node.get() : e_ptr->src;
				auto new_dst_ptr = *(e_ptr->dst) == old_data ? new_node.get() : e_ptr->dst;

				// Insert new and remove old
				edges_.emplace(std::make_shared<edge>(edge{new_src_ptr, new_dst_ptr, e_ptr->weight}));
				edges_.erase(e_ptr);
			}

			// Erase the old node
			nodes_.erase(old_it);

			return true;
		}

		// to be edited
		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			auto old_it = nodes_.find(old_data);
			auto new_it = nodes_.find(new_data);
			if (old_it == nodes_.end() or new_it == nodes_.end()) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}

			// Find all relevant nodes
			auto edge_ptrs = std::vector<std::shared_ptr<edge>>();
			std::copy_if(edges_.begin(),
			             edges_.end(),
			             std::back_inserter(edge_ptrs),
			             [&](auto const& e_ptr) {
				             return *(e_ptr->src) == old_data or *(e_ptr->dst) == old_data;
			             });

			// Merge the nodes
			for (auto const& e_ptr : edge_ptrs) {
				auto new_src_ptr = *(e_ptr->src) == old_data ? (*new_it).get() : e_ptr->src;
				auto new_dst_ptr = *(e_ptr->dst) == old_data ? (*new_it).get() : e_ptr->dst;

				struct edge new_edge = edge{new_src_ptr, new_dst_ptr, e_ptr->weight};
				edges_.erase(e_ptr);

				// If the new edge not exist, insert it
				if (edges_.find(new_edge) == edges_.end()) {
					edges_.emplace(std::make_shared<edge>(new_edge));
				}
			}

			// Remove the old node
			nodes_.erase(old_it);
		}
		//
		//
		auto erase_node(N const& value) -> bool{
			if (is_node(value)) {
				std::erase_if(edges_, [&](auto const& ed) { return *(ed->src) == value or *(ed->dst) == value; });
				nodes_.erase(nodes_.find(value));
				return true;
			}
			return false;
		}
		//
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool{
			if (is_node(src) and is_node(dst)){
				auto it = std::find_if(edges_.begin(), edges_.end(),
				                       [&](auto const& ed) { return *(ed->src) == src and *(ed->dst) == dst and ed->weight == weight; });
				if (it != edges_.end()) {
					edges_.erase(it);
					return true;
				}
				return false;

			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they "
			                         "don't exist in the graph");
		}


		//
		auto clear() noexcept -> void{
			nodes_.clear();
			edges_.clear();
		}
		//
		//		// Accessors
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			return nodes_.find(value) != nodes_.end();
		}
		//
		[[nodiscard]] auto empty() const -> bool {
			return nodes_.empty();
		}

		[[nodiscard]] auto nodes() const noexcept-> std::vector<N>{
			auto v = std::vector<N>{};
			std::for_each(nodes_.begin(), nodes_.end(), [&v](auto const& it) {
				v.emplace_back(*it);
			});
			return v;
		}

		// to be edit
		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool{
			if (is_node(src) and is_node(dst)) {
				auto connection = std::find_if(std::cbegin(edges_),
				                               std::cend(edges_),
				                               [src, dst](std::shared_ptr<edge> x) {
					                               return (*(x->from) == src and *(x->to) == dst);
				                               });
				return connection != std::cend(edges_);

			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node "
			                         "don't exist in the graph");
		}

		// to be test
		[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E>  {
			if (is_node(src) and is_node(dst)) {
				auto v = std::vector<E>{};
				std::for_each(edges_.begin(), edges_.end(), [&v, &src, &dst](auto const& it) {
					if (*(it->src) == src and *(it->dst) == dst) {
						v.emplace_back(it.weight);
					}
				});
				return v;
			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
			                         "don't exist in the graph");
		}


		[[nodiscard]] auto connections(N const& src) -> std::vector<N>{
			if (is_node(src)) {
				auto v = std::vector<N>{};
				std::for_each(edges_.begin(), edges_.end(), [&v, &src](auto const& it) {
					if (it.src == src) {
						v.emplace_back(*(it.dst));
					}
				});
				return v;
			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
			                         "exist in the graph");
		}

		// Iterator access

		//		// Comparisons, to be edit
		[[nodiscard]] auto operator==(graph const& other) -> bool{
			return std::equal(nodes_.begin(),
			                  nodes_.end(),
			                  other.nodes_.begin(),
			                  other.nodes_.end(),
			                  [](auto const& lhs, auto const& rhs) { return *lhs == *rhs; })
			       and std::equal(edges_.begin(),
			                      edges_.end(),
			                      other.edges_.begin(),
			                      other.edges_.end(),
			                      [](auto const& lhs, auto const& rhs) {
				                      return *(lhs->src) == *(rhs->src) and *(lhs->dst) == *(rhs->dst)
				                             and lhs->weight == rhs->weight;
			                      });
		}
		//
		//		// Extractor, to be tested
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream&{
			std::for_each(g.nodes_.begin(), g.nodes_.end(), [&](auto const& it_node){
				os << *it_node << "(\n";
				std::for_each(g.edges_.begin(), g.edges_.end(), [&](auto const& it_edge){
					if (*(it_edge->src) == *it_node) {
						os << "  " << *(it_edge->dst)<<" | "<<it_edge->weight<<"\n";
					}
				});
				os << ")\n";
			});
			return os;
		}

	private:

		struct node_cmp {
			using is_transparent = void;

			auto operator()(std::shared_ptr<N> const& a, std::shared_ptr<N> const& b) const -> bool {
				return *a < *b;
			}

			auto operator()(std::shared_ptr<N> const& a, N const& b) const -> bool {
				return *a < b;
			}

			auto operator()(N const& a, std::shared_ptr<N> const& b) const -> bool {
				return a < *b;
			}
		};

		struct edge_cmp {
			using is_transparent = void;

			auto operator()(std::shared_ptr<edge> const& a,
			                std::shared_ptr<edge> const& b) const noexcept -> bool {
				if (*(a->src) != *(b->src)) {
					return *(a->src) < *(b->src);
				}

				if (*(a->dst) != *(b->dst)) {
					return *(a->dst) < *(b->dst);
				}

				return a->weight < b->weight;
			}

			//			auto operator()(std::shared_ptr<edge> const& a, std::shared_ptr<edge> const& b) const
			//			   -> bool {
			//				return std::tie(*(a->src), *(a->dst), a->weight)
			//				       < std::tie(*(b->src), *(b->dst), b->weight);
			//			}

			// Compare edge ptr to edge struct
			//			auto operator()(std::shared_ptr<edge> const& a, struct edge const& b) const -> bool {
			//				return std::tie(*(a->src), *(a->dst), a->weight)
			//				       < std::tie(*(b.src), *(b.dst), b.weight);
			//			}
			//
			//			auto operator()(struct edge const& a, std::shared_ptr<edge> const& b) const -> bool {
			//				return std::tie(*(a.src), *(a.dst), a.weight)
			//				       < std::tie(*(b->src), *(b->dst), b->weight);
			//			}
			//
			//			// Compare edge ptr to value type
			//			auto operator()(std::shared_ptr<edge> const& a, struct value_type const& b) const
			//			   -> bool {
			//				return std::tie(*(a->src), *(a->dst), a->weight)
			//				       < std::tie(b.from, b.to, b.weight);
			//			}
			//
			//			auto operator()(struct value_type const& a, std::shared_ptr<edge> const& b) const
			//			   -> bool {
			//				return std::tie(a.from, a.to, a.weight)
			//				       < std::tie(*(b->src), *(b->dst), b->weight);
			//			}
		};

		std::set<std::shared_ptr<N>, node_cmp> nodes_;
		std::set<std::shared_ptr<edge>, edge_cmp> edges_;

	public:
		class iterator {
		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			// Iterator constructor
			iterator() = default;

			// Iterator source
			auto operator*() const -> reference {
				return value_type{*((*iter_)->src), *((*iter_)->dst), (*iter_)->weight};
			}

			// Iterator traversal
			auto operator++() -> iterator& {
				++iter_;
				return *this;
			}

			auto operator++(int) -> iterator {
				auto old = *this;
				++(*this);
				return old;
			}

			auto operator--() -> iterator& {
				--iter_;
				return *this;
			}

			auto operator--(int) -> iterator {
				auto old = *this;
				--(*this);
				return old;
			}

			// Iterator comparison
			auto operator==(iterator const& other) const -> bool {
				return iter_ == other.iter_;
			}

		private:
			friend class graph<N, E>;
			using edges_iterator = typename std::set<std::shared_ptr<edge>, edge_cmp>::iterator;

			edges_iterator iter_;

			explicit iterator(edges_iterator begin)
			: iter_{begin} {}

		};
		[[nodiscard]] auto begin() const -> iterator {
			return iterator(std::begin(edges_));
		}

		[[nodiscard]] auto end() const -> iterator{
			return iterator(std::end(edges_));
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator{
			return iterator{edges_.find(value_type{src, dst, weight})};
		}

		auto erase_edge(iterator i) -> iterator{
			return iterator{edges_.erase(i.it_)};
		}

		auto erase_edge(iterator i, iterator s) -> iterator{
			return iterator{edges_.erase(i.it_, s.it_)};
		}
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
