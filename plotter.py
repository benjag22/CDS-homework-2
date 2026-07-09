import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pandas import DataFrame
from pathlib import Path

CSV_PATH = "data/measurements.csv"
PLOTS_DIR = Path("plots")


def grouped_bar(df: DataFrame, value_col: str, ylabel: str, title: str, err_col: str | None = None, log: bool = False, int_labels: bool = False):
    types = sorted(df["type"].unique())
    graphs = sorted(df["graph"].unique())

    x = np.arange(len(types))
    width = 0.8 / len(graphs)

    fig, ax = plt.subplots(figsize=(10, 6))
    for i, graph in enumerate(graphs):
        values, errs = [], []
        for t in types:
            row = df[(df["type"] == t) & (df["graph"] == graph)]
            values.append(row[value_col].values[0] if not row.empty else 0)
            errs.append(row[err_col].values[0] if err_col and not row.empty else 0)
        bars = ax.bar(
            x + i * width, values, width, label=graph,
            yerr=errs if err_col else None, capsize=3,
            )
        if int_labels:
            labels = [f"{v:,.0f}" for v in values]
        else:
            labels = [f"{v:,.2f}" for v in values]
        ax.bar_label(bars, labels=labels, padding=3, fontsize=7)

    ax.set_xticks(x + width * (len(graphs) - 1) / 2)
    ax.set_xticklabels(types)
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    if log:
        ax.set_yscale("log")
    ax.legend(title="graph")
    fig.tight_layout()
    return fig


def main():
    PLOTS_DIR.mkdir(parents=True, exist_ok=True)
    df = pd.read_csv(CSV_PATH)

    size_df = df.drop_duplicates(subset=["graph", "type"])
    fig1 = grouped_bar(
        size_df, "size", "size (bytes)",
        "Data structure size by type and graph", log=True, int_labels=True,
    )
    fig1.savefig(PLOTS_DIR / "size_comparison.png", dpi=150)

    degree_df = df[df["fn"] == "degree"]
    fig2 = grouped_bar(
        degree_df, "t_mean", "time (s)",
        "degree() time by type and graph", err_col="t_stdev", log=True,
    )
    fig2.savefig(PLOTS_DIR / "degree_time_comparison.png", dpi=150)

    neighbours_df = df[df["fn"] == "neighbours"]
    fig3 = grouped_bar(
        neighbours_df, "t_mean", "time (s)",
        "neighbours() time by type and graph", err_col="t_stdev", log=True,
    )
    fig3.savefig(PLOTS_DIR / "neighbours_time_comparison.png", dpi=150)


if __name__ == "__main__":
    main()
