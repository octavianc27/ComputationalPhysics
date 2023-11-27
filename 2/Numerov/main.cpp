#include <iostream>
#include <cmath>
#include <tuple>
#include <fstream>
#include <string>

#define hbar 1
#define m 0.5
#define pi 3.14159

double potentialV(double r) {
    return r * r;
}

double f(int l, double r, double E, double (*potential)(double)) {
    return 2 * m / (hbar * hbar) * (potential(r) + hbar * hbar * l * (l + 1) / (2 * m * r * r) - E);
}

std::tuple<double, double, double, double> numerov(int l, double E, int N, double rmax) {
    std::ofstream out("numerov.out");

    double wavelength = 2 * pi * hbar / sqrt(2 * m * E);
    double h = rmax/N;

    double r = h;
    double wPrev = 0;
    double wCur = pow(h, l + 1);
    double uCur = pow(h, l + 1);

    while (r < rmax) {
        double wNext = 2 * wCur - wPrev + h * h * f(l, r, E, potentialV) * uCur;
        wPrev = wCur;
        wCur = wNext;
        uCur = wCur * (1 - h * h / 12. * f(l, r, E, potentialV));
        r += h;
        out << r << ' ' << uCur << std::endl;
    }
    double r1 = r;
    double ur1 = uCur;

    while (r < rmax + wavelength / 2) {
        double wNext = 2 * wCur - wPrev + h * h * f(l, r, E, potentialV) * uCur;
        wPrev = wCur;
        wCur = wNext;
        uCur = wCur / (1 - h * h / 12. * f(l, r, E, potentialV));
        r += h;
        out << r << ' ' << uCur << std::endl;
    }
    double r2 = r;
    double ur2 = uCur;

    return std::make_tuple(r1, r2, ur1, ur2);
}

int main() {
    std::tuple<double, double, double, double> result = numerov(0, 3, 1024, 3);
    double r1 = std::get<0>(result);
    double r2 = std::get<1>(result);
    double ur1 = std::get<2>(result);
    double ur2 = std::get<3>(result);

    std::cout << r1 << ' ' << ur1 << std::endl;
    std::cout << r2 << ' ' << ur2 << std::endl;
    return 0;
}
