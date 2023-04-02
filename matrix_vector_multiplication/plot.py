import matplotlib.pyplot as plt

OX = ["No optim", "mem reduce", "intrinsic", "asm", "openmp"]
GCC_time = [0.164468, 0.158625, 0.047510, 0.039676, 0.008943]
CLANG_time = [0.161525, 0.159011, 0.057641, 0.040528, 0.00963]
GCC_a = [GCC_time[0]/GCC_time[i] for i in range(0, len(GCC_time))]
CLANG_a = [CLANG_time[0]/CLANG_time[i] for i in range(0, len(CLANG_time))]
GCC_IPC = [1.71, 1.38, 1.34, 1.31, 1.15]
CLANG_IPC = [1.54, 1.40, 1.05, 1.30, 1.16]
GCC_cache_miss = [65.474, 50.951, 48.225, 48.76, 56.569]
CLANG_cache_miss = [63.221, 49.40, 49.208, 47.407, 54.130]

Xs = [i-0.2 for i in range(len(OX))]
Xs_gcc = [i-0.2 for i in Xs]
Xs_clang = [i+0.2 for i in Xs]

plt.plot(OX, GCC_time, label="Time (gcc)")
plt.plot(OX, CLANG_time, label="Time (clang)")
plt.legend()
plt.title("Time")
plt.show()

plt.bar(Xs_gcc, GCC_time, 0.4, label="Time (gcc)")
plt.bar(Xs_clang, CLANG_time, 0.4, label="Time (clang)")
plt.xticks(Xs, OX)
plt.legend()
plt.title("Time")
plt.show()

plt.plot(OX, GCC_a, label="Acceleration (gcc)")
plt.plot(OX, CLANG_a, label="Acceleration (clang)")
plt.legend()
plt.title("Acceleration")
plt.show()

plt.bar(Xs_gcc, GCC_a, 0.4, label="Time (gcc)")
plt.bar(Xs_clang, CLANG_a, 0.4, label="Time (clang)")
plt.xticks(Xs, OX)
plt.legend()
plt.title("Acceleration")
plt.show()

plt.plot(OX, GCC_IPC, label="IPC (gcc)")
plt.plot(OX, CLANG_IPC, label="IPC (clang)")
plt.legend()
plt.title("IPC")
plt.show()

plt.bar(Xs_gcc, GCC_IPC, 0.4, label="Time (gcc)")
plt.bar(Xs_clang, CLANG_IPC, 0.4, label="Time (clang)")
plt.xticks(Xs, OX)
plt.legend()
plt.title("IPC")
plt.show()

plt.plot(OX, GCC_cache_miss, label="cache-misses (gcc)")
plt.plot(OX, CLANG_cache_miss, label="cache-misses (clang)")
plt.legend()
plt.title("cache-misses")
plt.show()

plt.bar(Xs_gcc, GCC_cache_miss, 0.4, label="Time (gcc)")
plt.bar(Xs_clang, CLANG_cache_miss, 0.4, label="Time (clang)")
plt.xticks(Xs, OX)
plt.legend()
plt.title("cache-misses")
plt.show()


thread_X = [1, 2, 4, 8, 12]
thread_time = [0.040022, 0.020924, 0.012006, 0.010422, 0.009922]
thread_acc = [thread_time[0]/thread_time[i] for i in range(len(thread_time))]
plt.plot(thread_X, thread_time, label="Time (gcc)")
plt.legend()
plt.title("Thread time")
plt.show()

plt.plot(thread_X, thread_acc, label="Acceleration (gcc)")
plt.plot(thread_X, thread_X, label="Line 45")
plt.legend()
plt.title("Thread acceleration")
plt.show()