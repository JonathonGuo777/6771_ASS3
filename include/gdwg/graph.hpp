#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		struct value_type {
			N from;
			N to;
			E weight;
		};

		struct edge {
			N* src;
			N* dst;
			E weight;
		};

		class iterator;

		// Constructors
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

		// Copy Constructor
		graph(graph const& other){
			for (auto& it : other.nodes_) {
				nodes_.emplace(std::make_shared<N>(*it));
			}
			for (auto& it : other.edges_) {
				edges_.emplace(std::make_shared<edge>(*it));
			}
		}

		// Move Constructor
		graph(graph&& other) noexcept
		: nodes_{std::exchange(other.nodes_, std::set<std::shared_ptr<N>, node_cmp>())}
		, edges_{std::exchange(other.edges_, std::set<std::shared_ptr<edge>, edge_cmp>())} {}

		// Copy Assignment
		auto operator=(graph const& other) -> graph& {
			if (this == &other) {
				return *this;
			}
			auto obj = graph(other);
			std::swap(nodes_, obj.nodes_);
			std::swap(edges_, obj.edges_);
			return *this;
		}

		// Move Assignment
		auto operator=(graph&& other) noexcept -> graph& {
			std::swap(nodes_, other.nodes_);
			std::swap(edges_, other.edges_);
			return *this;
		}

		// Modifiers
		auto insert_node(N const& value) -> bool {
			return nodes_.emplace(std::make_shared<N>(value)).second;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {

			if (is_node(src) and is_node(dst)) {
				struct edge new_edge = {(*(nodes_.find(src))).get(), (*(nodes_.find(dst))).get(), weight};
				return edges_.emplace(std::make_shared<edge>(new_edge)).second;

			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
			                         "or dst node does not exist");
		}


		// relalce node
		auto replace_node(N const& old_data, N const& new_data) -> bool {
			auto old_iterer = nodes_.find(old_data);
			if (old_iterer != std::end(nodes_)){
				if (is_node(new_data)) {
					return false;
				}
				// insert new
				auto new_node = std::make_shared<N>(new_data);
				nodes_.emplace(new_node);
				// save all relevant edges
				auto edge_ptrs = std::vector<std::shared_ptr<edge>>();
				std::copy_if(edges_.begin(),
				             edges_.end(),
				             std::back_inserter(edge_ptrs),
				             [&](auto const& edge_it) {
					             return *(edge_it->src) == old_data or *(edge_it->dst) == old_data;
				             });
				// replace the nodes
				for (auto const& edge_it : edge_ptrs) {
//					auto new_src_ptr = *(edge_it->src) == old_data ? new_node.get() : edge_it->src;
//					auto new_dst_ptr = *(edge_it->dst) == old_data ? new_node.get() : edge_it->dst;
					auto new_src_ptr = edge_it->src;
					auto new_dst_ptr = edge_it->dst;
					if (*(edge_it->src) == old_data){
						new_src_ptr = new_node.get();
					}
					if (*(edge_it->dst) == old_data){
						new_dst_ptr = new_node.get();
					}
					// insert new and remove old
					edges_.emplace(std::make_shared<edge>(edge{new_src_ptr, new_dst_ptr, edge_it->weight}));
					edges_.erase(edge_it);
				}
				// erase old node
				nodes_.erase(old_iterer);
				return true;
			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
			                         "doesn't exist");
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			auto old_it = nodes_.find(old_data);
			auto new_it = nodes_.find(new_data);
			if (old_it == nodes_.end() or new_it == nodes_.end()) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}

			// save all relevant edges
			auto edge_ptrs = std::vector<std::shared_ptr<edge>>();
			std::copy_if(edges_.begin(),
			             edges_.end(),
			             std::back_inserter(edge_ptrs),
			             [&](auto const& e_ptr) {
				             return *(e_ptr->src) == old_data or *(e_ptr->dst) == old_data;
			             });

			// merge nodes
			for (auto const& e_ptr : edge_ptrs) {
				auto new_src_ptr = e_ptr->src;
				auto new_dst_ptr = e_ptr->dst;
				if (*(e_ptr->src) == old_data){
					new_src_ptr = (*new_it).get();
				}
				if (*(e_ptr->dst) == old_data){
					new_dst_ptr = (*new_it).get();
				}
				struct edge new_edge = edge{new_src_ptr, new_dst_ptr, e_ptr->weight};
				edges_.erase(e_ptr);

				//	check if edge already exists
				if (edges_.find(new_edge) == edges_.end()) {
					edges_.emplace(std::make_shared<edge>(new_edge));
				}
			}
			// delete old node
			nodes_.erase(old_it);
		}

		auto erase_node(N const& value) -> bool {
			if (is_node(value)) {
				std::erase_if(edges_, [&](auto const& ed) { return *(ed->src) == value or *(ed->dst) == value; });
				nodes_.erase(nodes_.find(value));
				return true;
			}
			return false;
		}

		// log(n) + e
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
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

		auto erase_edge(iterator i) -> iterator {
			if (i == end() or i == iterator{}) {
				return end();
			}
			return iterator{edges_.erase(i.iter_)};
		}

		auto erase_edge(iterator i, iterator s) -> iterator {
			return iterator{edges_.erase(i.iter_, s.iter_)};
		}

		auto clear() noexcept -> void {
			nodes_.clear();
			edges_.clear();
		}


		// Accessors
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			if (nodes_.find(value) == nodes_.end()) {
				return false;
			}
			return true;
		}

		[[nodiscard]] auto empty() const -> bool {
			return nodes_.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (is_node(src) and is_node(dst)) {
				auto connection = std::find_if(edges_.begin(),
				                               edges_.end(),
				                               [src, dst](std::shared_ptr<edge> x) {
					                               return (*(x->src) == src and *(x->dst) == dst);
				                               });
				return connection != edges_.end();

			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node "
			                         "don't exist in the graph");
		}

		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto v = std::vector<N>{};
			for (auto const& node_it : nodes_){
				v.emplace_back(*node_it);
			}
			return v;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if (is_node(src) and is_node(dst)) {
				auto v = std::vector<E>{};
				for (auto const& edge_it : edges_) {
					if (*(edge_it->src) == src and *(edge_it->dst) == dst) {
						v.emplace_back(edge_it->weight);
					}
				}
				return v;
			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
			                         "don't exist in the graph");
		}

		// log(n)+log(e)
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			return iterator{edges_.find(value_type{src, dst, weight})};
		}

		// log(n) + log(e)
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (is_node(src)) {
				auto v = std::vector<N>{};
				std::for_each(edges_.begin(), edges_.end(), [&v, &src](auto const& edge_it) {
					if (*(edge_it->src) == src) {
						v.emplace_back(*(edge_it->dst));
					}
				});
				return v;
			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
			                         "exist in the graph");
		}

		// Iterator
		[[nodiscard]] auto begin() const -> iterator {
			return iterator{edges_.begin()};
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator{edges_.end()};
		}

		// Comparision
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			if (other.nodes_.size() == nodes_.size() and other.edges_.size() == edges_.size()) {
				return std::equal(nodes_.begin(), nodes_.end(), other.nodes_.begin(),
				                  [](auto const& x, auto const& y) { return *x == *y; })
				       and std::equal(edges_.begin(), edges_.end(), other.edges_.begin(),
				                      [](auto const& x, auto const& y) {
					                      return *(x->src) == *(y->src)
					                         and *(x->dst) == *(y->dst)
					                         and x->weight== y->weight; });
			}
			return false;
		}

	private:
		struct node_cmp {
			using is_transparent = void;

			auto operator()(std::shared_ptr<N> const& x, std::shared_ptr<N> const& y) const -> bool {
				return *x < *y;
			}

			auto operator()(std::shared_ptr<N> const& x, N const& y) const -> bool {
				return *x < y;
			}

			auto operator()(N const& x, std::shared_ptr<N> const& y) const -> bool {
				return x < *y;
			}
		};
		

		struct edge_cmp {
			using is_transparent = void;

			auto operator()(std::shared_ptr<edge> const& x, std::shared_ptr<edge> const& y) const
			   -> bool {
				return std::tie(*(x->src), *(x->dst), x->weight)
				       < std::tie(*(y->src), *(y->dst), y->weight);
			}

			auto operator()(std::shared_ptr<edge> const& x, struct edge const& y) const -> bool {
				return std::tie(*(x->src), *(x->dst), x->weight)
				       < std::tie(*(y.src), *(y.dst), y.weight);
			}

			auto operator()(struct edge const& x, std::shared_ptr<edge> const& y) const -> bool {
				return std::tie(*(x.src), *(x.dst), x.weight)
				       < std::tie(*(y->src), *(y->dst), y->weight);
			}

			auto operator()(std::shared_ptr<edge> const& x, struct value_type const& y) const
			   -> bool {
				return std::tie(*(x->src), *(x->dst), x->weight)
				       < std::tie(y.from, y.to, y.weight);
			}

			auto operator()(struct value_type const& x, std::shared_ptr<edge> const& y) const
			   -> bool {
				return std::tie(x.from, x.to, x.weight)
				       < std::tie(*(y->src), *(y->dst), y->weight);
			}
		};

		std::set<std::shared_ptr<N>, node_cmp> nodes_;
		std::set<std::shared_ptr<edge>, edge_cmp> edges_;

		// Hidden Friend: Extractor
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			for (auto const& node_it : g.nodes_)   {
				os << *node_it << " (\n";
				for (auto const& edge_it : g.edges_) {
					if (*(edge_it->src) == *node_it) {
						os << "  " << *(edge_it->dst) << " | " << edge_it->weight << "\n";
					}
				}
				os << ")\n";
			}
			return os;
		}

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
	};
} // namespace gdwg

#endif // GDWG_GRAPH_HPP