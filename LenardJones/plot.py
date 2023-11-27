import numpy as np
import matplotlib.pyplot as plt

h = 0.001
data = np.genfromtxt("cmake-build-debug/sigma.txt")
plt.plot(data[:, 0], data[:, 1])
plt.show()
