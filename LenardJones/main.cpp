#include <iostream>
#include <cmath>
#include <tuple>
#include <fstream>
#include <string>

#define factor 1/6.12 //hbar^2/(2*m)
#define pi 3.14159
#define epsilon 5.9
#define rho 1
#define alpha 6.12

double potentialV(double r) {
    return epsilon * (pow(rho / r, 12) - 2 * pow(rho / r, 6));
}

double f(int l, double r, double E, double (*potential)(double)) {
    return 2 / factor * (potential(r) + factor * l * (l + 1) / (2 * r * r) - E);
}

double besselJ(int l, double x) {
    if (l == 0) {
        return sin(x) / x;
    }
    if (l == 1) {
        return sin(x) / (x * x) - cos(x) / x;
    }
    return ((2 * l - 1) / x) * besselJ(l - 1, x) - besselJ(l - 2, x);
}

double besselN(int l, double x) {
    if (l == 0) {
        return -cos(x) / x;
    }
    if (l == 1) {
        return -cos(x) / (x * x) - sin(x) / x;
    }
    return (2 * l - 1) / x * besselN(l - 1, x) - besselN(l - 2, x);
}

std::tuple<double, double, double, double> numerov(int l, double E, double h, double rmax) {
    double wavelength = 2 * pi * sqrt(factor * E);
    double rmin = 0.55 * rho;

    double r = rmin + h;
    double C = sqrt(epsilon * alpha / 25);

    double uPrev = exp(-C * std::pow(r - h, -5));
    double uCur = exp(-C * std::pow(r, -5));

    auto w_func = [&, h](double r, double u) {
        return (1.0 - (1.0 / 12.0) * h * h * f(l, r, E, potentialV)) * u;
    };

    auto u_func = [&, h](double r, double w) {
        return std::pow(1.0 - (1.0 / 12.0) * h * h * f(l, r, E, potentialV), -1) * w;
    };

    double wPrev = w_func(r - h, uPrev);
    double wCur = w_func(r, uCur);

    bool r_max_reached = false;
    bool finished = false;
    double r1, ur1, r2, ur2;

    while (!finished) {
        double wNext = 2 * wCur - wPrev + h * h * f(l, r, E, potentialV) * uCur;
        r = r + h;
        wPrev = wCur;
        wCur = wNext;
        uCur = u_func(r, wCur);

        if (r > rmax && !r_max_reached) {
            r1 = r;
            ur1 = uCur;
            r_max_reached = true;
        }
        if (r > rmax + wavelength / 2) {
            r2 = r;
            ur2 = uCur;
            finished = true;
        }
    }

    return std::make_tuple(r1, r2, ur1, ur2);
}

double calculateDelta(double E, int l, double h, double rmax) {
    std::tuple<double, double, double, double> result = numerov(l, E, h, rmax);

    double k = sqrt(E / factor);

    double r1 = std::get<0>(result);
    double r2 = std::get<1>(result);
    double ur1 = std::get<2>(result);
    double ur2 = std::get<3>(result);

    double K = (r1 * ur2) / (r2 * ur1);
    return atan(K * besselJ(l, k * r1) - besselJ(l, k * r2)/(K * besselN(l, k * r1) - besselN(l, k * r2)));
}

double calculateSigma(double E, int lmax, double h, double rmax) {
    double k = sqrt(E / factor);
    double sum = 0;
    for (int l = 0; l <= lmax; l++)
        sum += pow(sin(calculateDelta(E, l, h, rmax)), 2) * (2 * l + 1);
    sum *= 4 * 3.14159 / k;
    return sum;
}

int main() {
    double h = 0.0001;
    double rmax = 5.0;

    std::ofstream outFile("sigma.txt");

    for (double E = 0.2; E <= 3.5; E += 0.02) {
        double sigma = calculateSigma(E, 7, h, rmax);

        std::cout << "E = " << E << ", sigma = " << sigma << std::endl;

        outFile << E << " " << sigma << std::endl;
    }

    return 0;
}
