#pragma once

#include <filesystem>

#include "common.hpp"
#include "complementary/utils.hpp"
#include "sdsl/pemb.hpp"

class PlanarEmbedding {
    sdsl::pemb<> m_pemb;

    using size_type = sdsl::pemb<>::size_type;

public:
    explicit PlanarEmbedding(const std::filesystem::path &file_path) {
        namespace fs = std::filesystem;

        fs::path out_file_path = file_path;
        out_file_path.replace_extension(PEMB_EXT);

        if (fs::exists(out_file_path)) {
            std::cout << "planar embedding for " << file_path.stem() << " found, reloading" << std::endl;
            std::ifstream infile(out_file_path, std::ios::binary);
            m_pemb.load(infile);
            infile.close();
            return;
        }

        std::cout << "planar embedding for " << file_path.stem() << " not found, building" << std::endl;

        std::cout << "building graph for planar embedding" << std::endl;
        Graph graph = read_graph_from_file(file_path.c_str());

        std::cout << "building planar embedding" << std::endl;
        sdsl::pemb pemb(graph);

        std::ofstream outfile(out_file_path, std::ios::binary);
        pemb.serialize(outfile);
        outfile.close();

        std::cout << "reloading planar embedding" << std::endl;
        std::ifstream infile(out_file_path, std::ios::binary);
        m_pemb.load(infile);
        infile.close();
    }

    [[nodiscard]] bool degree(const size_type u) {
        return m_pemb.degree(u);
    }

    [[nodiscard]] bool neighbours(const size_type u, const size_type v) {
        if (u >= m_pemb.vertices() || v >= m_pemb.vertices()) {
            return false;
        }

        size_type next = m_pemb.first(v);
        while (next < 2 * m_pemb.edges()) {
            if (m_pemb.vertex(m_pemb.mate(next)) == u) {
                return true;
            }

            next = m_pemb.next(next);
        }

        return false;
    }
};
