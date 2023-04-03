import matplotlib.pyplot as plt

OX = ["No optim", "tiling", "openmp", "branch"]
GCC_time = [3.151042,3.845146, 1.272879, 0.964160]
CLANG_time = [3.536035, 3.555679, 1.262614, 1.094300]
GCC_a = [GCC_time[0]/GCC_time[i] for i in range(0, len(GCC_time))]
CLANG_a = [CLANG_time[0]/CLANG_time[i] for i in range(0, len(CLANG_time))]
GCC_IPC = [3.46, 3.49, 1.34, 1.38]
CLANG_IPC = [2.13, 2.16,  1.05, 1.38]
GCC_cache_miss = [53.980, 23.739 , 4.943, 0.251]
CLANG_cache_miss = [56.527  , 21.249, 3.259 ,  0.843]

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