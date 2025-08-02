import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV
df = pd.read_csv("PS_2.csv")

# Sort by Matrix_Size and Threads for consistent plotting
df = df.sort_values(by=["Matrix_Size", "Threads"])

# --- Plot 1: Execution Time vs Number of Threads ---
plt.figure(figsize=(10, 6))
for size in df["Matrix_Size"].unique():
    subset = df[df["Matrix_Size"] == size]
    plt.plot(subset["Threads"], subset["Time(s)"],
             marker="o", label=f"Matrix Size = {size}x{size}")

plt.title("Execution Time vs Number of Threads (Matrix Addition)")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (s)")
plt.xticks(df["Threads"].unique())
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("PS_2_Execution_Time_vs_Number_of_Threads.png")
plt.show()

# --- Plot 2: Speedup vs Number of Threads ---
plt.figure(figsize=(10, 6))
for size in df["Matrix_Size"].unique():
    subset = df[df["Matrix_Size"] == size].copy()
    time_1_thread = subset[subset["Threads"] == 1]["Time(s)"].values[0]
    subset["Speedup"] = time_1_thread / subset["Time(s)"]
    plt.plot(subset["Threads"], subset["Speedup"],
             marker="o", label=f"Matrix Size = {size}x{size}")

# Ideal speedup line
max_threads = df["Threads"].max()
plt.plot(range(1, max_threads + 1), range(1, max_threads + 1),
         "k--", label="Ideal Linear Speedup")

plt.title("Speedup vs Number of Threads (Matrix Addition)")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.xticks(df["Threads"].unique())
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("PS_2_Speedup_vs_Number_of_Threads.png")
plt.show()

# --- Plot 3: Execution Time vs Matrix Size (Fixed Number of Threads) ---
fixed_thread = 4  # Change as needed
fixed_df = df[df["Threads"] == fixed_thread]

plt.figure(figsize=(10, 6))
plt.plot(fixed_df["Matrix_Size"], fixed_df["Time(s)"],
         marker="o", color="purple")
plt.title(f"Execution Time vs Matrix Size (Threads = {fixed_thread})")
plt.xlabel("Matrix Size (N x N)")
plt.ylabel("Execution Time (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("PS_2_Execution_Time_vs_Matrix_Size.png")
plt.show()
