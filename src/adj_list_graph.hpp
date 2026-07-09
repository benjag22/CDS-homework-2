#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "testable.hpp"

class AdjListGraph : Testable {
    std::vector<std::vector<int32_t>> m_adj;

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
    }

    [[nodiscard]] int32_t degree(const int32_t u) override {
        return m_adj[u].size();
    }

    [[nodiscard]] bool neighbors(const int32_t u, const int32_t v) override {
        return std::ranges::binary_search(m_adj[u], v);
    }
};
