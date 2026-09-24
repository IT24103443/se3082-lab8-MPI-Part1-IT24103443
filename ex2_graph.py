import matplotlib.pyplot as plt

processors = [1, 2, 4, 8]
times = [0.03, 0.04, 0.05, 0.11] 

# Calculate Actual Speedup (T1 / Tp)
speedup = [times[0]/t for t in times]
ideal_speedup = processors

plt.figure(figsize=(14, 6))

# Time Plot
plt.subplot(1, 2, 1)
plt.plot(processors, times, marker='o', linestyle='-', color='#1f77b4', linewidth=2, markersize=8)
plt.title('Execution Time vs Number of Processors')
plt.xlabel('Number of Processors (P)')
plt.ylabel('Execution Time (seconds)')
plt.xticks(processors)
plt.grid(True, linestyle='--', alpha=0.7)

# Speedup Plot
plt.subplot(1, 2, 2)
plt.plot(processors, speedup, marker='o', linestyle='-', color='#d62728', linewidth=2, label='Actual Speedup')
plt.plot(processors, ideal_speedup, marker='s', linestyle='--', color='#2ca02c', linewidth=2, label='Ideal Linear Speedup')
plt.title('Speedup vs Number of Processors')
plt.xlabel('Number of Processors (P)')
plt.ylabel('Speedup (S = T1 / Tp)')
plt.xticks(processors)
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)

plt.tight_layout()
plt.show()