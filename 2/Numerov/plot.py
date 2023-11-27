import numpy as np
import matplotlib.pyplot as plt

h = 0.001
data = np.genfromtxt("cmake-build-debug/numerov.out")
plt.plot(data[:, 0], data[:, 1])
r = data[:, 0]
plt.plot(r, np.exp(h ** 2 / 2) * r * np.exp(-r ** 2 / 2))
plt.show()
