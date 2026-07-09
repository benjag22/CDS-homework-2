#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "common.hpp"
#include "testable.hpp"
#include "k2tree/buildtree.h"
#include "k2tree/compressleaves.h"
#include "k2tree/kTree.h"

class K2Tree : Testable {
    TREP *m_trep;

public:
    explicit K2Tree(const std::filesystem::path &file_path) {
        namespace fs = std::filesystem;

        fs::path out_file_path = file_path;
        out_file_path.replace_extension(K2_EXT);

        if (fs::exists(out_file_path)) {
            std::cout << "k2 tree file for " << file_path.stem() << " not, reloading" << std::endl;
            m_trep = loadTreeRepresentation(const_cast<char *>(out_file_path.c_str()));
            return;
        }

        std::cout << "k2 tree file for " << file_path.stem() << " not found, building" << std::endl;

        std::ifstream infile(file_path);
        if (!infile) {
            std::cerr << "failed to open " << file_path << std::endl;
            exit(1);
        }

        std::ofstream outfile(out_file_path, std::ios::binary);
        if (!outfile) {
            std::cerr << "failed to open " << out_file_path << std::endl;
            exit(1);
        }

        std::cout << "mapping edges for k2 tree" << std::endl;

        int32_t num_vertices;
        int32_t num_edges;
        infile >> num_vertices >> num_edges;

        const int32_t total_edges = num_edges * 2;

        outfile.write(reinterpret_cast<const char *>(&num_vertices), sizeof(int32_t));
        outfile.write(reinterpret_cast<const char *>(&total_edges), sizeof(int64_t));

        int32_t src;
        int32_t dst;
        int32_t current_node = -1;

        for (int32_t i = 0; i < total_edges; i++) {
            infile >> src >> dst;

            while (current_node < src) {
                current_node++;
                const int32_t marker = -(current_node + 1);
                outfile.write(reinterpret_cast<const char *>(&marker), sizeof(int32_t));
            }

            const int32_t val = dst + 1;
            outfile.write(reinterpret_cast<const char *>(&val), sizeof(int32_t));
        }

        while (current_node < num_vertices - 1) {
            current_node++;
            const int32_t marker = -(current_node + 1);
            outfile.write(reinterpret_cast<const char *>(&marker), sizeof(int32_t));
        }

        infile.close();
        outfile.close();

        std::cout << "building k2 tree" << std::endl;

        const char *out_file_path_cstr = out_file_path.c_str();
        const char *argv_bt[] = {
            ".",
            // graph file path
            out_file_path_cstr,
            // basename
            out_file_path_cstr,
            // K1, must be 4
            "4",
            // K2, must be 2
            "2",
            // max level K1, 5 = smaller, 6 = biggest
            "6",
            // speeddup, 22 if large graph
            "22",
        };

        if (build_tree(7, argv_bt) != 0) {
            std::cerr << "build_tree() failed" << std::endl;
            exit(1);
        }

        std::cout << "compressing k2 tree leaves" << std::endl;

        const char *argv_cl[] = {
            ".",
            out_file_path_cstr,
            "4000000",
        };

        if (compress_leaves(3, argv_cl) != 0) {
            std::cerr << "compress_leaves() failed" << std::endl;
        }

        std::cout << "reloading k2 tree" << std::endl;
        m_trep = loadTreeRepresentation(const_cast<char *>(out_file_path_cstr));
    }

    ~K2Tree() override {
        destroyTreeRepresentation(m_trep);
    }

    [[nodiscard]] int32_t degree(const int32_t u) override {
        return compactTreeAdjacencyList(m_trep, u)[0];
    }

    [[nodiscard]] bool neighbors(const int32_t u, const int32_t v) override {
        return compactTreeCheckLink(m_trep, u, v);
    }
};
