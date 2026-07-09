# Compact Data Structures - Homework 2

## Requirements

- Linux machine
- CMake >= 3.16
- C++ compiler compatible with C++ 20
- Python >= 3.12 and `uv` if you want to plot results

## Compile

```bash
./build.sh
```

or

```bash
mkdir -p build
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cd build
make
```

## Execute

The program assumes you have put the graphs you wish to test in the `graphs` directory. These graphs must follow the
format specified at https://www.inf.udec.cl/~jfuentess/datasets/graphs.php.

```bash
./build/cds-hw2
```

The results will be written in the `data` directory.

## Plot results

You may run the following to plot the results. Plots will be stored in the `plots` directory.

```bash
uv sync
uv run plotter.py
```
