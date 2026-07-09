#pragma once

#include <filesystem>

#include "common.hpp"
#include "complementary/utils.hpp"
#include "sdsl/pemb.hpp"

class PlanarEmbedding : Testable {
    sdsl::pemb<> m_pemb;
    uint64_t m_size_in_bytes = 0;

    using size_type = sdsl::pemb<>::size_type;

public:
    explicit PlanarEmbedding(const std::filesystem::path &file_path) {
        namespace fs = std::filesystem;

        fs::path out_file_path = file_path;
        out_file_path.replace_extension(PEMB_EXT);

        if (fs::exists(out_file_path)) {
            std::cout << "planar embedding for " << file_path.stem() << " found, reloading" << std::endl;
            std::ifstream infile(out_file_path, std::ios::binary);

            infile.seekg(0, std::ios::end);
            m_size_in_bytes = infile.tellg();
            infile.seekg(0, std::ios::beg);

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

    [[nodiscard]] int32_t degree(const int32_t u) override {
        return static_cast<int32_t>(m_pemb.degree(u));
    }

    [[nodiscard]] bool neighbors(const int32_t u, const int32_t v) override {
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

    [[nodiscard]] uint64_t size_in_bytes() const override {
        return m_size_in_bytes;
    }
};
