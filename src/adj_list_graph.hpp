#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "testable.hpp"

class AdjListGraph : Testable {
    std::vector<std::vector<int32_t> > m_adj;
    uint64_t m_size_in_bytes = 0;

public:
    explicit AdjListGraph(const std::filesystem::path &file_path) {
        std::ifstream infile(file_path);
        if (!infile) {
            std::cerr << "failed to open " << file_path << std::endl;
            exit(1);
        }

        int32_t num_vertices, num_edges;
        infile >> num_vertices >> num_edges;

        const int32_t total_edges = num_edges * 2;

        m_adj.resize(num_edges);

        for (int32_t i = 0; i < total_edges; i++) {
            int32_t src, dst;
            infile >> src >> dst;
            m_adj[src].push_back(dst);
        }

        for (auto &list: m_adj) {
            std::ranges::sort(list);
        }

        m_size_in_bytes = sizeof(std::vector<std::vector<int32_t> >)
            + sizeof(std::vector<int32_t>) * m_adj.size();

        for (const auto &list: m_adj) {
            m_size_in_bytes += sizeof(int32_t) * list.size();
        }
    }

    [[nodiscard]] int32_t degree(const int32_t u) override {
        return static_cast<int32_t>(m_adj[u].size());
    }

    [[nodiscard]] bool neighbors(int32_t u, int32_t v) override {
        if (m_adj[u].size() > m_adj[v].size()) {
            std::swap(u, v);
        }

        return std::ranges::binary_search(m_adj[u], v);
    }

    [[nodiscard]] uint64_t size_in_bytes() const override {
        return m_size_in_bytes;
    }
};
