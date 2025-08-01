import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv("PS_2.csv")

# Rename columns if needed
df.columns = ['Steps', 'Threads', 'Time', 'Pi']

# Sort data
df = df.sort_values(by=["Steps", "Threads"])

# --- Plot 1: Execution Time vs Number of Threads ---
plt.figure(figsize=(10, 6))
for step in df['Steps'].unique():
    subset = df[df['Steps'] == step]
    plt.plot(subset['Threads'], subset['Time'], marker='o', label=f"Steps = {step}")

plt.title("Execution Time vs Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (s)")
plt.xticks(df['Threads'].unique())
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("PS_2_Execution_Time_vs_Number_of_Threads.png")
plt.show()

# --- Plot 2: Speedup vs Number of Threads ---
plt.figure(figsize=(10, 6))
for step in df['Steps'].unique():
    subset = df[df['Steps'] == step].copy()
    time_1_thread = subset[subset['Threads'] == 1]['Time'].values[0]
    subset['Speedup'] = time_1_thread / subset['Time']
    plt.plot(subset['Threads'], subset['Speedup'], marker='o', label=f"Steps = {step}")

# Ideal linear speedup line
max_threads = df['Threads'].max()
plt.plot(range(1, max_threads+1), range(1, max_threads+1), 'k--', label="Ideal Linear Speedup")

plt.title("Speedup vs Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.xticks(df['Threads'].unique())
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("PS_2_Speedup_vs_Threads.png")
plt.show()

# --- Plot 3: Execution Time vs Problem Size (for fixed thread count) ---
fixed_thread = 4  # Change if needed
fixed_df = df[df['Threads'] == fixed_thread]

plt.figure(figsize=(10, 6))
plt.plot(fixed_df['Steps'], fixed_df['Time'], marker='o', color='green')
plt.title(f"Execution Time vs Problem Size (Threads = {fixed_thread})")
plt.xlabel("Problem Size (Steps)")
plt.ylabel("Execution Time (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("PS_2_Execution_Time_vs_Problem_Size.png")
plt.show()
