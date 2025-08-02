import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV
df = pd.read_csv("PS_1.csv")

# Sort by Vector_Size and Threads for consistent plotting
df = df.sort_values(by=["Vector_Size", "Threads"])

# --- Plot 1: Execution Time vs Number of Threads ---
plt.figure(figsize=(10, 6))
for vec_size in df["Vector_Size"].unique():
    subset = df[df["Vector_Size"] == vec_size]
    plt.plot(subset["Threads"], subset["Time(s)"], marker="o", label=f"Vector Size = {vec_size}")

plt.title("Execution Time vs Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (s)")
plt.xticks(df["Threads"].unique())
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("PS_1_Execution_Time_vs_Number_of_Threads.png")
plt.show()

# --- Plot 2: Speedup vs Number of Threads ---
plt.figure(figsize=(10, 6))
for vec_size in df["Vector_Size"].unique():
    subset = df[df["Vector_Size"] == vec_size].copy()
    time_1_thread = subset[subset["Threads"] == 1]["Time(s)"].values[0]
    subset["Speedup"] = time_1_thread / subset["Time(s)"]
    plt.plot(subset["Threads"], subset["Speedup"], marker="o", label=f"Vector Size = {vec_size}")

# Ideal linear speedup
max_threads = df["Threads"].max()
plt.plot(range(1, max_threads + 1), range(1, max_threads + 1), "k--", label="Ideal Linear Speedup")

plt.title("Speedup vs Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.xticks(df["Threads"].unique())
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("PS_1_Speedup_vs_Number_of_Threads.png")
plt.show()

fixed_thread = 4  
fixed_df = df[df["Threads"] == fixed_thread]

plt.figure(figsize=(10, 6))
plt.plot(fixed_df["Vector_Size"], fixed_df["Time(s)"], marker="o", color="purple")
plt.title(f"Execution Time vs Vector Size (Threads = {fixed_thread})")
plt.xlabel("Vector Size")
plt.ylabel("Execution Time (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("PS_1_Execution_Time_vs_Vector_Size.png")
plt.show()
