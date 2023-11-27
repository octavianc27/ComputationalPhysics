import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("cmake-build-debug/fractal.txt")
b = np.log(data[:,0])
N = np.log(data[:,1])

fit = np.polyfit(b, N, 1)
print(fit)

plt.scatter(b, N)
plt.plot(np.linspace(np.min(b), np.max(b), 100), fit[1] + fit[0]*np.linspace(np.min(b), np.max(b), 100))

plt.show()
