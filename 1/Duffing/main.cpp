#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#define pi 3.14159

const double m = 1;
const double damping_gamma = 0.1;
const double a = 1.0 / 4.0;
const double b = 1.0 / 2.0;
const double f0 = 2.0;
const double omega = 2.4;

const double x0 = 0.5;
const double dxdt0 = 0.0;

const int num_steps = 150000;
const double dt = 2 * pi / (50 * omega);

double d2xdt(double x, double dxdt, double t) {
    double force = -damping_gamma * dxdt + 2 * a * x - 4 * b * x * x * x + f0 * cos(omega * t);
    return force / m;
}

void eulerMethod(double &x, double &dxdt, double &t, double dt) {
    double d2xdt2 = d2xdt(x, dxdt, t);
    dxdt += d2xdt2 * dt;
    x += dxdt * dt;
    t += dt;
}

int main() {
    std::ofstream out("duffing.txt");
    std::ofstream fractalOut("fractal.txt");

    if (!out.is_open()) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    double x = x0;
    double dxdt = dxdt0;
    double t = 0.0;

    std::vector<std::pair<double, double>> attractor_points;

    out << "Time Position Velocity" << std::endl;
    out << t << ' ' << x << ' ' << dxdt << std::endl;

    for (int step = 0; step < num_steps; step++) {
        eulerMethod(x, dxdt, t, dt);
        attractor_points.push_back(std::make_pair(x, dxdt));

        out << t << ' ' << x << ' ' << dxdt << std::endl;
        std::cout << "Time: " << t << ", Position: " << x << std::endl;
    }

    int num_cells = 2;

    while (num_cells <= 128) {
        float grid_size = 6.0 / num_cells;
        std::vector<std::vector<bool>> cell_array(num_cells, std::vector<bool>(num_cells, false));

        for (const auto &point: attractor_points) {
            int cell_x = static_cast<int>((point.first + 3) / grid_size);
            int cell_y = static_cast<int>((point.second + 3) / grid_size);
            if (cell_x >= 0 && cell_x < num_cells && cell_y >= 0 && cell_y < num_cells) {
                cell_array[cell_x][cell_y] = true;
            }
        }

        int filled_cells = 0;
        for (int i = 0; i < num_cells; i++) {
            for (int j = 0; j < num_cells; j++) {
                if (cell_array[i][j]) {
                    filled_cells++;
                }
            }
        }

        fractalOut << grid_size<<' '<<filled_cells<<std::endl;

        num_cells *= 2;
    }
    return 0;
}