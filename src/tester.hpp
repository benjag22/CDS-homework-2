#pragma once

#include <concepts>
#include <filesystem>
#include <iostream>

#include "adj_list_graph.hpp"
#include "int_generator.hpp"
#include "k2_tree.hpp"
#include "planar_embedding.hpp"

template<typename T>
concept TestableImpl = std::derived_from<T, Testable>;

static constexpr int RUNS = 32;
static constexpr int REPS = 1000;
static constexpr int PRECISION = 9;

struct Measurement {
    std::array<double, RUNS> times{};
    double time_mean = 0;
    double time_stdev = 0;
    std::array<double, 5> quartiles{};

    void calculate() {
        for (const double &time: times) {
            time_mean += time;
        }
        time_mean /= RUNS;

        for (const double &time: times) {
            const double dev = time - time_mean;
            time_stdev += dev * dev;
        }

        time_stdev /= static_cast<double>(RUNS - 1);
        time_stdev = std::sqrt(time_stdev);

        constexpr size_t n = RUNS;

        std::ranges::sort(times);

        quartiles[0] = times.front();
        quartiles[4] = times.back();

        size_t part = n / 2;
        quartiles[2] = (times[part - 1] + times[part]) / 2.0;

        part = n / 4;
        quartiles[1] = 0.25 * times[part - 1] + 0.75 * times[part];
        part = 3 * n / 4;
        quartiles[3] = 0.75 * times[part - 1] + 0.25 * times[part];
    }
};

inline std::ostream &operator<<(std::ostream &out, const Measurement &measurement) {
    return out << std::fixed << std::setprecision(PRECISION)
        << measurement.time_mean << ','
        << measurement.time_stdev << ','
        << measurement.quartiles[0] << ','
        << measurement.quartiles[1] << ','
        << measurement.quartiles[2] << ','
        << measurement.quartiles[3] << ','
        << measurement.quartiles[4];
}

inline void display_progress(uint64_t runs, uint64_t total);

template<TestableImpl T>
void run_tests(std::ofstream &outfile, const char *graph_name, T &ds, const char *ds_name) {
    static_assert(std::is_base_of_v<Testable, T>, "T must implement Testable");

    Measurement degree_measurement, neighbours_measurement;
    std::chrono::duration<double, std::nano> elapsed_time{};
    int_generator<int32_t> int_gen(0, ds.vertices());

    for (int32_t i = 0; i < RUNS; i++) {
        display_progress(i + 1, RUNS);

        const int32_t u = int_gen();
        const int32_t v = int_gen();

        auto begin_time = std::chrono::high_resolution_clock::now();
        for (int32_t j = 0; j < REPS; j++) {
            ds.degree(u);
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        elapsed_time = end_time - begin_time;
        degree_measurement.times[i] = elapsed_time.count();
        degree_measurement.times[i] /= REPS;

        begin_time = std::chrono::high_resolution_clock::now();
        for (int32_t j = 0; j < REPS; j++) {
            ds.neighbors(u, v);
        }
        end_time = std::chrono::high_resolution_clock::now();

        elapsed_time = end_time - begin_time;
        neighbours_measurement.times[i] = elapsed_time.count();
        neighbours_measurement.times[i] /= REPS;
    }
    std::cout << std::endl;

    degree_measurement.calculate();
    neighbours_measurement.calculate();

    outfile
        << graph_name << ','
        << ds_name << ','
        << ds.size_in_bytes() << ','
        << "degree" << ','
        << degree_measurement << '\n';

    outfile
        << graph_name << ','
        << ds_name << ','
        << ds.size_in_bytes() << ','
        << "neighbours" << ','
        << neighbours_measurement << '\n';

    outfile << std::flush;
}

inline void run_tests(const std::vector<std::filesystem::path> &file_paths) {
    namespace fs = std::filesystem;

    const fs::path data_dir_path = fs::path(__builtin_FILE()).parent_path().parent_path() / "data";
    const fs::path outfile_path = data_dir_path / "measurements.csv";
    std::ofstream outfile(outfile_path);
    outfile << "graph,type,size,fn,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4\n";

    for (const auto &file_path: file_paths) {
        const std::string &graph_name_str = file_path.stem().string();
        const char *graph_name = graph_name_str.c_str();
        std::cout << "--- building trees for " << graph_name << std::endl;

        AdjListGraph adj_list_graph(file_path);
        K2Tree k2_tree(file_path);
        PlanarEmbedding pemb(file_path);

        std::cout << "--- running tests for adjacency list graph" << std::endl;
        run_tests(outfile, graph_name, adj_list_graph, "adj");

        std::cout << "--- running tests for k2 tree" << std::endl;
        run_tests(outfile, graph_name, k2_tree, "k2");

        std::cout << "--- running tests for planar embedding" << std::endl;
        run_tests(outfile, graph_name, pemb, "pemb");

        std::cout << std::endl;
    }

    outfile.close();
}

inline void display_progress(const uint64_t runs, const uint64_t total) {
    using std::cout;

    const double progress = runs / static_cast<double>(total);
    constexpr uint64_t width = 70;
    const size_t p = width * progress;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(3) << progress * 100;
    const std::string &progress_string = stream.str();

    cout << "[";
    for (size_t i = 0; i < width; i++) {
        if (i < p)
            cout << '=';
        else if (i == p)
            cout << '>';
        else
            cout << ' ';
    }

    cout << "] " << progress_string << "%\r";
    cout.flush();
}
