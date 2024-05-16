#include "./naturalspline.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

double naturalsplinevalue(int n, vector<double>& x, vector<double>& f, double xx, int& st) {
    if (n < 1) {
        st = 1;
        return 0.0;
    }
    if (xx < x[0] || xx > x[n]) {   
        st = 3;
        return 0.0;
    }

    st = 0;
    for (int i = 0; i < n; ++i) {
        for (int k = i + 1; k <= n; ++k) {
            if (x[i] == x[k]) {
                st = 2;
                return 0.0;
            }
        }
    }

    vector<double> d(n + 1, 0.0);
    vector<double> b(n - 1, 0.0);
    vector<double> c(n, 0.0);

    if (n > 1) {
        double z = x[2];
        double y = z - x[1];
        z -= x[0];
        double u = f[1];
        b[0] = y / z;
        d[1] = 6 * ((f[2] - u) / y - (u - f[0]) / (x[1] - x[0])) / z;

        z = x[n - 2];
        y = x[n - 1] - z;
        z = x[n] - z;
        u = f[n - 1];
        c[n - 1] = y / z;
        d[n - 1] = 6 * ((f[n] - u) / (x[n] - x[n - 1]) - (u - f[n - 2]) / y) / z;

        for (int i = 2; i < n - 1; ++i) {
            z = x[i];
            y = x[i + 1] - z;
            z -= x[i - 1];
            u = f[i];
            b[i - 1] = y / (y + z);
            c[i] = 1 - b[i - 1];
            d[i] = 6 * ((f[i + 1] - u) / y - (u - f[i - 1]) / z) / (y + z);
        }

        u = 2.0;
        y = d[1] / u;
        d[1] = y;
        for (int i = 1; i < n - 1; ++i) {
            double z = b[i - 1] / u;
            b[i - 1] = z;
            u = 2.0 - z * c[i];
            y = (d[i + 1] - y * c[i]) / u;
            d[i + 1] = y;
        }

        u = d[n - 1];
        for (int i = n - 2; i >= 1; --i) {
            u = d[i] - u * b[i - 1];
            d[i] = u;
        }
    }

    int i = 0;
    while (!(xx >= x[i] && xx <= x[i + 1])) {
        ++i;
    }

    double y = x[i + 1] - x[i];
    double z = d[i + 1];
    double u = d[i];
    double a0 = f[i];
    double a1 = (f[i + 1] - f[i]) / y - (2 * u + z) * y / 6;
    double a2 = u / 2;
    double a3 = (z - u) / (6 * y);
    double result = a3;
    z = xx - x[i];
    for (int j = 2; j >= 0; --j) {
        result = result * z + (j == 0 ? a0 : (j == 1 ? a1 : a2));
    }

    return result;
}
