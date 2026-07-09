#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "tester.hpp"

namespace fs = std::filesystem;

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

    run_tests(graph_file_paths);

    return 0;
}
