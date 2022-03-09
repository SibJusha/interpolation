#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "function_values.h"

void get_coefficients (std::vector<double>& coefficients, int n,
                       std::vector<double>& X)
{
    for (int i = 0; i < n; i++) {
        double coeff = 0;
        for (int j = 0; j <= i; j++) {
            double multi = 1;
            for (int k = 0; k <= i; k++) {
                if (k == j) continue;
                multi *= (X[j] - X[k]);
            }
            multi = get_value(X[j]) / multi;
            coeff += multi;
        }
        coefficients.insert(coefficients.end(), coeff);
    }
}

double get_value (std::vector<double>& coeffs, std::vector<double>& X,
                  double x)
{
    int n = coeffs.size();
    double result = 0;
    if (n > 0) {
        result += coeffs[0];
    }
    for (int i = 1; i < n; i++) {
        double multi = 1;
        for (int j = 0; j < i; j++) {
            multi *= (x - X[j]);
        }
        result += multi * coeffs[i];
    }
    return result;
}

int main() {
    std::ifstream in;
    std::ofstream out, debug;
    in.open("input.txt");
    debug.open("debug.txt");
    out.open("output.txt", std::ios::ate);

    int n1, n2;
    double a, b, H, h;
    bool cheby;
    in >> cheby >> a >> b >> n1 >> n2;
    std::vector<double> variable;
    if (cheby) {
        for (int i = 0; i < n1; i++) {
            double x = (a + b) / 2 + (b - a) / 2 * cos((2*i + 1)*M_PI/2/(n1+1));
            debug << x << std::endl;
            variable.insert(variable.end(), x);
        }
    } else {
        H = (b - a) / n1;
        for (int i = 0; i < n1; i++) {
            variable.insert(variable.end(), a + i * H);
        }
    }
    h = (b - a) / n2;
    std::vector<double> coefficients;
    get_coefficients(coefficients, n1, variable);
    double max = -1;
    double max_x = -1;
    for (int i = 0; i < n2; i++) {
        double x = get_value(coefficients, variable, a + i * h);
        debug << a + i * h << "\t\t" << x << std::endl;
        double y = get_value(a + i * h);
        if (std::abs(y - x) > max) {
            max = std::abs(y - x);
            max_x = a + i * h;
        }
    }

    out << max << std::endl << "x = "<< max_x << std::endl;

    in.close();
    debug.close();
    out.close();
    return 0;
}
