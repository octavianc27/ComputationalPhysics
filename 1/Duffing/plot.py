import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("cmake-build-debug/duffing.txt", skip_header=1)
t = data[:, 0]
x = data[:, 1]
dxdt = data[:, 2]

plt.plot(t[:2500], x[:2500])
plt.xlabel("t")
plt.ylabel("x")
plt.show()

plt.figure()
plt.scatter(x[::50], dxdt[::50], s=1)
plt.ylabel("dxdt")
plt.xlabel("x")
plt.show()