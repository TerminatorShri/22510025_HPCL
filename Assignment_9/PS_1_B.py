import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
df = pd.read_csv("PS_1_B.csv")

# Execution time comparison plot
plt.figure(figsize=(8, 5))
for mode in df["Mode"].unique():
    subset = df[df["Mode"] == mode]
    plt.plot(subset["InputX"], subset["ExecutionTime"], marker="o", label=mode)

plt.xlabel("Input X")
plt.ylabel("Execution Time (s)")
plt.title("Execution Time: Sequential vs Speculative")
plt.legend()
plt.grid(True)
plt.savefig("execution_time_comparison.png")
plt.show()

# Speedup (Seq / Spec)
seq_times = df[df["Mode"] == "Sequential"].set_index("InputX")["ExecutionTime"]
spec_times = df[df["Mode"] == "Speculative"].set_index("InputX")[
    "ExecutionTime"]

speedup = seq_times / spec_times

plt.figure(figsize=(8, 5))
plt.plot(speedup.index, speedup.values, marker="o", color="purple")
plt.xlabel("Input X")
plt.ylabel("Speedup (Seq/Spec)")
plt.title("Speculative Speedup over Sequential")
plt.axhline(y=1.0, color="red", linestyle="--", label="No Speedup")
plt.legend()
plt.grid(True)
plt.savefig("speedup.png")
plt.show()

# Wasted Work %
plt.figure(figsize=(8, 5))
subset = df[df["Mode"] == "Speculative"]
plt.bar(subset["InputX"], subset["WastedWorkPercent"],
        color="orange", width=0.5)

plt.xlabel("Input X")
plt.ylabel("Wasted Work (%)")
plt.title("Wasted Work in Speculative Execution")
plt.ylim(0, 100)
plt.grid(axis="y")
plt.savefig("wasted_work.png")
plt.show()
