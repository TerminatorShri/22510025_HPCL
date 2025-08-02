import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# CONFIGURATION
STATIC_CSV = "PS_3_Static.csv"
DYNAMIC_CSV = "PS_3_Dynamic.csv"
sns.set(style="whitegrid")
plt.rcParams.update({'font.size': 10})

# Load datasets
df_static = pd.read_csv(STATIC_CSV)
df_dynamic = pd.read_csv(DYNAMIC_CSV)

df_static['Schedule'] = 'Static'
df_dynamic['Schedule'] = 'Dynamic'
df = pd.concat([df_static, df_dynamic], ignore_index=True)

df["Time(s)"] = df["Time(s)"].round(6)

# === 1. SPEEDUP vs THREADS (static + dynamic in subplots) ===


def plot_speedup_subplot():
    fig, axes = plt.subplots(1, 2, figsize=(14, 6), sharey=True)
    for idx, (data, sched) in enumerate([(df_static, "Static"), (df_dynamic, "Dynamic")]):
        ax = axes[idx]
        for chunk in sorted(data['Chunk_Size'].unique()):
            subset = data[data['Chunk_Size'] == chunk].sort_values('Threads')
            base_time = subset[subset['Threads'] == 1]['Time(s)'].values[0]
            speedup = base_time / subset['Time(s)']
            ax.plot(subset['Threads'], speedup,
                    marker='o', label=f'Chunk {chunk}')
        ax.set_title(f"Speedup vs Threads ({sched})")
        ax.set_xlabel("Threads")
        if idx == 0:
            ax.set_ylabel("Speedup")
        ax.grid(True)
        ax.legend(title="Chunk Size")

    fig.suptitle("Speedup Comparison: Static vs Dynamic", fontsize=14)
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig("PS_3_Speedup_Combined.png")
    plt.show()

# === 2. TIME vs CHUNK SIZE per thread (subplots) ===


def plot_time_vs_chunk_subplots():
    thread_counts = sorted(df['Threads'].unique())
    cols = 3
    rows = (len(thread_counts) + cols - 1) // cols
    fig, axes = plt.subplots(rows, cols, figsize=(5 * cols, 4 * rows))
    axes = axes.flatten()

    for i, thread in enumerate(thread_counts):
        ax = axes[i]
        subset_static = df_static[df_static['Threads'] == thread]
        subset_dynamic = df_dynamic[df_dynamic['Threads'] == thread]

        sns.lineplot(data=subset_static, x='Chunk_Size',
                     y='Time(s)', ax=ax, marker='o', label='Static')
        sns.lineplot(data=subset_dynamic, x='Chunk_Size',
                     y='Time(s)', ax=ax, marker='o', label='Dynamic')

        ax.set_title(f"Threads = {thread}")
        ax.set_xlabel("Chunk Size")
        ax.set_ylabel("Time (s)")
        ax.legend()

    # Hide unused subplots
    for j in range(i + 1, len(axes)):
        fig.delaxes(axes[j])

    fig.suptitle("Time vs Chunk Size for each Thread Count", fontsize=14)
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig("PS_3_Time_vs_Chunk_AllThreads.png")
    plt.show()

# === 3. STATIC vs DYNAMIC per chunk size ===


def compare_sched_by_chunk_subplots():
    chunk_sizes = sorted(df['Chunk_Size'].unique())
    cols = 3
    rows = (len(chunk_sizes) + cols - 1) // cols
    fig, axes = plt.subplots(rows, cols, figsize=(5 * cols, 4 * rows))
    axes = axes.flatten()

    for i, chunk in enumerate(chunk_sizes):
        ax = axes[i]
        subset = df[df['Chunk_Size'] == chunk]
        sns.lineplot(data=subset, x='Threads', y='Time(s)',
                     hue='Schedule', marker='o', ax=ax)
        ax.set_title(f"Chunk Size = {chunk}")
        ax.set_xlabel("Threads")
        ax.set_ylabel("Time (s)")

    # Hide unused subplots
    for j in range(i + 1, len(axes)):
        fig.delaxes(axes[j])

    fig.suptitle(
        "Static vs Dynamic: Time vs Threads for each Chunk Size", fontsize=14)
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig("PS_3_Compare_Sched_By_Chunk.png")
    plt.show()


# === Run all plots ===
plot_speedup_subplot()
plot_time_vs_chunk_subplots()
compare_sched_by_chunk_subplots()
