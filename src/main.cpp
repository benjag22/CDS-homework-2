#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "build_trees.hpp"

namespace fs = std::filesystem;

uint32_t exp_pow(const uint32_t base, const uint32_t pow) {
    uint32_t result = 1;
    for (uint32_t i = 0; i < pow; i++)
        result *= base;
    return result;
}

int main() {
    const fs::path graphs_dir_path = fs::path(__builtin_FILE()).parent_path().parent_path() / "graphs";
    std::vector<fs::path> graph_file_paths;
    std::error_code ec;

    for (const auto &entry: fs::directory_iterator(graphs_dir_path, ec)) {
        if (entry.is_regular_file() && entry.path().extension() == GRAPH_EXT) {
            graph_file_paths.push_back(entry.path());
        }
    }

    if (ec || graph_file_paths.empty()) {
        std::cerr << "no graphs found at " << graphs_dir_path << std::endl;
        exit(1);
    }

    for (const auto &file_path: graph_file_paths) {
        build_k2tree(file_path);
        build_pemb(file_path);
    }

    return 0;
}
