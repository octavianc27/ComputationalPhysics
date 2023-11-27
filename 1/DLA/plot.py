import numpy as np
import matplotlib.pyplot as plt

coordinates = np.genfromtxt("cmake-build-debug/dla.txt")
lattice_size = int(coordinates[0][0])
coordinates = coordinates[1:]

matrix = np.zeros((lattice_size, lattice_size), dtype=int)
for x, y in coordinates:
    matrix[int(x), int(y)] = 1

plt.imshow(matrix, cmap="viridis", origin="lower")
plt.title("Diffusion-Limited Aggregation")
plt.show()

radii = np.linalg.norm(np.subtract(coordinates, np.array([lattice_size / 2, lattice_size / 2])), axis=1)
radii_squared = np.square(radii)

radii_squared_sum = np.cumsum(radii_squared)
n = 1 + np.array(range(len(radii)))
radius = np.sqrt(radii_squared_sum / n)

fit = np.polyfit(np.log(radius[1:]), np.log(n[1:]), 1)
print("Fractal dimension: ", fit[0])

plt.plot(np.log(n[1:]), np.log(radius[1:]))
plt.xlabel("n")
plt.ylabel("Radius of gyration")
plt.show()
