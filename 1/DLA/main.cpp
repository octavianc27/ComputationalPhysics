#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>

//Diffusion limited aggregation simulation
const int lattice_size = 150;
const int num_points = 9000;

bool lattice[lattice_size][lattice_size];

int main() {
    std::ofstream out("dla.txt");
    lattice[lattice_size / 2][lattice_size / 2] = true;
    out<<lattice_size<<' '<<num_points<<std::endl; // Print lattice size and number of points

    out << lattice_size / 2 << ' ' << lattice_size / 2 << std::endl;

    for (int i = 0; i < num_points; i++) {
        std::cout<<i<<std::endl;

        // Release a walker from a random edge position
        int edge = std::rand() % 4; // 0: top, 1: bottom, 2: left, 3: right
        int x, y;

        switch (edge) {
            case 0: // Top edge
                x = std::rand() % lattice_size;
                y = 0;
                break;
            case 1: // Bottom edge
                x = std::rand() % lattice_size;
                y = lattice_size - 1;
                break;
            case 2: // Left edge
                x = 0;
                y = std::rand() % lattice_size;
                break;
            case 3: // Right edge
                x = lattice_size - 1;
                y = std::rand() % lattice_size;
                break;
            default:
                break;
        }

        // Perform random walk until particle neighbors an occupied site

        while (!lattice[x][y]) {
            int direction = std::rand() % 4; // 0: up, 1: down, 2: left, 3: right
            int new_x = x;
            int new_y = y;

            // Update new_x and new_y based on the chosen direction
            switch (direction) {
                case 0: // Up
                    new_y = (y > 0) ? y - 1 : y;
                    break;
                case 1: // Down
                    new_y = (y < lattice_size - 1) ? y + 1 : y;
                    break;
                case 2: // Left
                    new_x = (x > 0) ? x - 1 : x;
                    break;
                case 3: // Right
                    new_x = (x < lattice_size - 1) ? x + 1 : x;
                    break;
                default:
                    break;
            }

            if (lattice[new_x][new_y]) {
                out << x << ' ' << y << std::endl;
                lattice[x][y] = true;
                break;
            }

            // Update x and y to the new positions
            x = new_x;
            y = new_y;
        }


    }
    return 0;
}