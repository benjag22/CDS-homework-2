#pragma once
#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

class AdjacencyList {
    std::vector<std::vector<uint32_t>> adj;
public:
    explicit AdjacencyList(const std::vector<std::pair<uint32_t,uint32_t>>& edges) {
        uint32_t maxVertex = 0;

        for (auto [u,v] : edges)
            maxVertex = std::max({maxVertex, u, v});

        adj.resize(maxVertex + 1);

        for (auto [u,v] : edges) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        for (auto& list : adj)
            std::ranges::sort(list);
    }
    [[nodiscard]] size_t degree(const uint32_t u) const {
        return adj[u].size();
    }

    [[nodiscard]] bool neighbors(const uint32_t u, const uint32_t v) const {
        return std::ranges::binary_search(adj[u], v);
    }
};
