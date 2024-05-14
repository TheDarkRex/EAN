#include "./naturalspline.h"

int st = 0;

double naturalsplinevalue(int n, std::vector<double> x, std::vector<double> f, double xx, int* st) {
        int i, k;
        double u, y, z;
        bool found;
        std::vector<double> d(n + 1);
        std::vector<double> b(n - 1);
        std::vector<double> c(n - 1);
        double a[4];

        if (n < 1) {
            *st = 1;
        } else if (xx < x[0] || xx > x[n]) {
            *st = 3;
        } else {
            st = 0;
            i = -1;
            do {
                i++;
                for (k = i + 1; k <= n; k++) {
                    if (x[i] == x[k]) {
                        *st = 2;
                        break;
                    }
                }
            } while ((i < n - 1) && (*st != 2));
        }

    double result = 0.0;

    if (st == 0) {
        d[0] = 0;
        d[n] = 0;

        if (n > 1) {
            z = x[2];
            y = z - x[1];
            z = z - x[0];
            u = f[1];
            b[0] = y / z;
            d[1] = 6 * ((f[2] - u) / y - (u - f[0]) / (x[1] - x[0])) / z;
            z = x[n - 2];
            y = x[n - 1] - z;
            z = x[n] - z;
            u = f[n - 1];
            c[n - 2] = y / z;
            d[n - 1] = 6 * ((f[n] - u) / (x[n] - x[n - 1]) - (u - f[n - 2]) / y) / z;

            for (i = 1; i <= n - 2; i++) {
                z = x[i];
                y = x[i + 1] - z;
                z = z - x[i - 1];
                u = f[i];
                b[i] = y / (y + z);
                c[i - 1] = 1 - b[i];
                d[i] = 6 * ((f[i + 1] - u) / y - (u - f[i - 1]) / z) / (y + z);
            }

            u = 2;
            i = 0;
            y = d[1] / u;
            d[1] = y;

            if (n > 2) {

                do {
                    i++;
                    z = b[i] / u;
                    b[i] = z;
                    u = 2 - z * c[i];
                    y = (d[i + 1] - y * c[i]) / u;
                    d[i + 1] = y;
                } while (i < n - 2);
            }
        }

        u = d[n - 1];

        for (i = n - 2; i >= 1; i--) {
            u = d[i] - u * b[i];
            d[i] = u;
        }

        found = false;
        i = -1;

        do {
            i++;
            if (xx >= x[i] && xx <= x[i + 1]) {
                found = true;
            }

        } while (!found);
        y = x[i + 1] - x[i];
        z = d[i + 1];
        u = d[i];
        a[0] = f[i];
        a[1] = (f[i + 1] - f[i]) / y - (2 * u + z) * y / 6;
        a[2] = u / 2;
        a[3] = (z - u) / (6 * y);
        y = a[3];
        z = xx - x[i];

        for (i = 2; i >= 0; i--) {
            y = y * z + a[i];
        }
        result = y;
    }      

return result;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
}