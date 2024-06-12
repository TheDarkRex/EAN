#include "./naturalspline.h"


#include <iostream>
#include <vector>
#include <mpfr.h>
#include <Interval.h>
#include <string>

using namespace std;



void float_to_interval(double xx, vector<double> x, vector<double> f, vector<Interval<mpreal>>& interval_x, vector<Interval<mpreal>>& interval_f, Interval<mpreal>& interval_xx)
{
    mpreal::set_default_prec(256);

    string xx_double = to_string(xx);

    // cout << xx << endl;
    // cout << xx_double << endl;

    interval_xx.a = LeftRead<mpreal>(xx_double);
    interval_xx.b = RightRead<mpreal>(xx_double);

    cout << interval_xx.a << endl;
    // cout << Interval<mpreal>(6.0, 6.0).a << endl;

    for (int i = 0; i < (int) interval_x.size(); i++)
    {   
        string x_double = to_string(x[i]);
        string f_double = to_string(f[i]);

        interval_x[i].a = LeftRead<mpreal>(x_double);
        interval_x[i].b = RightRead<mpreal>(x_double);

        interval_f[i].a = LeftRead<mpreal>(f_double);
        interval_f[i].b = RightRead<mpreal>(f_double);
    }
}


void interval_naturalsplinevalue(int n, vector<Interval<mpreal>>& x, vector<Interval<mpreal>>& f, Interval<mpreal>& xx, Interval<mpreal>& result, int& st)
{
    mpreal::set_default_prec(256);

    if (n < 1) {
        st = 1;
        return;
    } else if (xx.a < x[0].a && xx.b < x[0].b || xx.a > x[n].a && xx.b > x[n].b) {
        st = 3;
        return;
    } else {
        st = 0;
        for (int i = 0; i < n; ++i) {
            for (int k = i + 1; k <= n; ++k) {
                if (x[i].a == x[k].a && x[i].b == x[k].b) {
                    st = 2;
                    return;
                }
            }
        }
    }

    // Initialize variables
    std::vector<Interval<mpreal>> d(n + 1), b(n - 1), c(n);
    Interval<mpreal> u, y, z, temp1, temp2;
    Interval<mpreal> a[4];

    if (st == 0) {
        d[0] = {0.0, 0.0};
        d[n] = {0.0, 0.0};

        if (n > 1) {
            z = x[2];

            y = ISub(z, x[1]);

            z = ISub(z, x[0]);

            u = f[1];

            b[1] = IDiv(y, z);

            d[1] = IMul(IDiv(ISub(IDiv(ISub(f[2], u), y), IDiv(ISub(u, f[0]), ISub(x[1], x[0]))), z), Interval<mpreal>(6.0, 6.0));

            z = x[n-2];

            y = ISub(x[n-1], z);

            z = ISub(x[n], z);

            u = f[n-1];

            c[n-1] = IDiv(y, z);

            d[n-1] = IMul(IDiv(ISub(IDiv(ISub(f[n], u), ISub(x[n], x[n-1])), IDiv(ISub(u, f[n-2]), y)), z), Interval<mpreal>(6.0, 6.0));

            for (int i = 2; i <= n - 2; ++i) {
                z = x[i];
                y = ISub(x[i+1], z);
                z = ISub(z, x[i-1]);
                u = f[i];

                temp1 = IAdd(y, z);
                b[i] = IDiv(y, temp1);
                c[i] = ISub(Interval<mpreal>(1.0, 1.0), b[i]);

                d[i] = IMul(IDiv(ISub(IDiv(ISub(f[i+1], u), y), IDiv(ISub(u, f[i-1]), z)), IAdd(y, z)), Interval<mpreal>(6.0, 6.0));
            }

            u = Interval<mpreal>(2.0, 2.0);
            int i = 0;
            y = IDiv(d[1], u);
            d[1] = y;

            if (n > 2) {
                do {
                    i++;
                    z = IDiv(b[i], u);
                    b[i] = z;

                    u = ISub(Interval<mpreal>(2.0, 2.0), IMul(z, c[i+1]));

                    y = IDiv(ISub(d[i+1], IMul(y, c[i+1])), u);

                    d[i+1] = y;
                } while (i < n - 2);
            }

            u = d[n - 1];

            for (int i = n - 2; i >= 1; --i) {
                u = ISub(d[i], IMul(u, b[i]));
                d[i] = u;
            }
        }

        cout << "test" << endl;

        bool found = false;
        int i = -1;

        do {
            i++;
            if (xx.a >= x[i].a && xx.b >= x[i].b && xx.a <= x[i + 1].a && xx.b <= x[i+1].b) {
                found = true;
            }
        } while (!found);

        y = ISub(x[i + 1], x[i]);

        z = d[i + 1];
        u = d[i];

        a[0] = f[i];
        a[1] = ISub(IDiv(ISub(f[i+1], f[i]), y), IDiv(IMul(IAdd(IMul(Interval<mpreal>(2.0, 2.0), u), z), y), Interval<mpreal>(6.0, 6.0)));
        a[2] = IDiv(u, Interval<mpreal>(2.0, 2.0));
        a[3] = IDiv(ISub(z, u), IMul(Interval<mpreal>(6.0, 6.0), y));

        y = a[3];
        z = xx - x[i];

        for (i = 2; i >= 0; --i) {
            y = IAdd(IMul(y, z), a[i]);
        }

        result = y;
    }
}


void naturalsplinevalue(int n, vector<mpfr_t>& x, vector<mpfr_t>& f, mpfr_t& xx, mpfr_t& result, int& st) {
    // cout << mpfr_get_d(xx, MPFR_RNDN) << endl;
    // cout << mpfr_get_d(x[0], MPFR_RNDN) << endl;

    //

    //

    mpfr_set_default_prec(256);

    if (n < 1) {
        st = 1;
        return;
    } else if (mpfr_cmp(xx, x[0]) < 0 || mpfr_cmp(xx, x[n]) > 0) {
        st = 3;
        return;
    } else {
        st = 0;
        for (int i = 0; i < n; ++i) {
            for (int k = i + 1; k <= n; ++k) {
                if (mpfr_cmp(x[i], x[k]) == 0) {
                    st = 2;
                    return;
                }
            }
        }
    }

        // Initialize variables
    vector<mpfr_t> d(n + 1), b(n - 1), c(n);
    mpfr_t u, y, z, temp1, temp2;
    mpfr_t a[4];

    for (int i = 0; i < n + 1; ++i) mpfr_init(d[i]);

    for (int i = 0; i < n - 1; ++i) {
        mpfr_init(b[i]);
    }

    for (int i = 0; i < n; i++)
    {
        mpfr_init(c[i]);
    }
    

    for (int i = 0; i < 4; ++i) mpfr_init(a[i]);

    mpfr_init(u);
    mpfr_init(y);
    mpfr_init(z);
    mpfr_init(temp1);
    mpfr_init(temp2);

    

    

    if (st == 0) {
        mpfr_set_si(d[0], 0, MPFR_RNDN);
        mpfr_set_si(d[n], 0, MPFR_RNDN);

        if (n > 1) {
            
            mpfr_set(z, x[2], MPFR_RNDN);
            mpfr_sub(y, z, x[1], MPFR_RNDN);
            mpfr_sub(z, z, x[0], MPFR_RNDN);
            mpfr_set(u, f[1], MPFR_RNDN);
            mpfr_div(b[1], y, z, MPFR_RNDN);


            mpfr_sub(d[1], f[2], u, MPFR_RNDN);
            mpfr_div(d[1], d[1], y, MPFR_RNDN);
            mpfr_sub(temp1, u, f[0], MPFR_RNDN);
            mpfr_sub(temp2, x[1], x[0], MPFR_RNDN);
            mpfr_div(temp1, temp1, temp2, MPFR_RNDN);
            mpfr_sub(d[1], d[1], temp1, MPFR_RNDN);
            mpfr_mul_si(d[1], d[1], 6, MPFR_RNDN);
            mpfr_div(d[1], d[1], z, MPFR_RNDN);


            mpfr_set(z, x[n-2], MPFR_RNDN);


            mpfr_sub(y, x[n-1], z, MPFR_RNDN);


            mpfr_sub(z, x[n], z, MPFR_RNDN);


            mpfr_set(u, f[n-1], MPFR_RNDN);


            mpfr_div(c[n-1], y, z, MPFR_RNDN);


            mpfr_sub(d[n-1], f[n], u, MPFR_RNDN);
            mpfr_sub(temp1, x[n], x[n-1], MPFR_RNDN);
            mpfr_div(d[n-1], d[n-1], temp1, MPFR_RNDN);
            mpfr_sub(temp2, u, f[n-2], MPFR_RNDN);
            mpfr_div(temp2, temp2, y, MPFR_RNDN);
            mpfr_sub(d[n-1], d[n-1], temp2, MPFR_RNDN);
            mpfr_mul_si(d[n-1], d[n-1], 6, MPFR_RNDN);
            mpfr_div(d[n-1], d[n-1], z, MPFR_RNDN);
            

            for (int i = 2; i <= n - 2; ++i) {
                mpfr_set(z, x[i], MPFR_RNDN);
                mpfr_sub(y, x[i+1], z, MPFR_RNDN);
                mpfr_sub(z, z, x[i-1], MPFR_RNDN);
                mpfr_set(u, f[i], MPFR_RNDN);


                mpfr_add(temp1, y, z, MPFR_RNDN);
                mpfr_div(b[i], y, temp1, MPFR_RNDN);


                mpfr_si_sub(c[i], 1, b[i], MPFR_RNDN);

                mpfr_sub(d[i], f[i+1], u, MPFR_RNDN);
                mpfr_div(d[i], d[i], y, MPFR_RNDN);
                mpfr_sub(temp1, u, f[i-1], MPFR_RNDN);
                mpfr_div(temp1, temp1, z, MPFR_RNDN);
                mpfr_sub(d[i], d[i], temp1, MPFR_RNDN);
                mpfr_mul_si(d[i], d[i], 6, MPFR_RNDN);
                mpfr_add(temp2, y, z, MPFR_RNDN);
                mpfr_div(d[i], d[i], temp2, MPFR_RNDN);
            }

            mpfr_set_si(u, 2, MPFR_RNDN);
            int i = 0;
            mpfr_div(y, d[1], u, MPFR_RNDN);
            mpfr_set(d[1], y, MPFR_RNDN);

            // cout << mpfr_get_d(d[0], MPFR_RNDN) << endl;
            // cout << mpfr_get_d(d[1], MPFR_RNDN) << endl;
            // cout << mpfr_get_d(d[2], MPFR_RNDN) << endl;
            // cout << mpfr_get_d(d[3], MPFR_RNDN) << endl;
            // cout << mpfr_get_d(d[4], MPFR_RNDN) << endl;

            if (n > 2) {
                do {
                    i++;

                    mpfr_div(z, b[i], u, MPFR_RNDN);
                    mpfr_set(b[i], z, MPFR_RNDN);
                    mpfr_mul(temp1, z, c[i+1], MPFR_RNDN);
                    mpfr_si_sub(u, 2, temp1, MPFR_RNDN);

                    mpfr_mul(temp2, y, c[i+1], MPFR_RNDN);
                    mpfr_sub(y, d[i+1], temp2, MPFR_RNDN);
                    mpfr_div(y, y, u, MPFR_RNDN);
                    mpfr_set(d[i+1], y, MPFR_RNDN);
                    cout << mpfr_get_d(d[i+1], MPFR_RNDN) << endl;
                } while (i < n - 2);
            }

            mpfr_set(u, d[n - 1], MPFR_RNDN);
            
            for (int i = n - 2; i >= 1; --i) {
                mpfr_mul(temp1, u, b[i], MPFR_RNDN);
                //cout << mpfr_get_d(d[i], MPFR_RNDN) << endl;
                mpfr_sub(u, d[i], temp1, MPFR_RNDN);
                //cout << mpfr_get_d(u, MPFR_RNDN) << endl;
                mpfr_set(d[i], u, MPFR_RNDN);
                //cout << mpfr_get_d(d[i], MPFR_RNDN) << endl;
            }

            //cout << mpfr_get_d(d[i], MPFR_RNDN) << endl;
        }

        // cout << "test" << endl;

        bool found = false;
        int i = -1;
        do {
            i++;
            if (mpfr_cmp(xx, x[i]) >= 0 && mpfr_cmp(xx, x[i + 1]) <= 0) {
                found = true;
            }
        } while (!found);

        mpfr_sub(y, x[i + 1], x[i], MPFR_RNDN);

        mpfr_set(z, d[i + 1], MPFR_RNDN);
        mpfr_set(u, d[i], MPFR_RNDN);

        mpfr_set(a[0], f[i], MPFR_RNDN);

        mpfr_sub(a[1], f[i + 1], f[i], MPFR_RNDN);
        mpfr_div(a[1], a[1], y, MPFR_RNDN);

        mpfr_mul_si(temp1, u, 2, MPFR_RNDN);


        mpfr_add(temp1, temp1, z, MPFR_RNDN);
        mpfr_mul(temp1, temp1, y, MPFR_RNDN);
        mpfr_div_si(temp1, temp1, 6, MPFR_RNDN);
        mpfr_sub(a[1], a[1], temp1, MPFR_RNDN);


        mpfr_div_si(a[2], u, 2, MPFR_RNDN);

        mpfr_sub(temp1, z, u, MPFR_RNDN);
        mpfr_mul_si(temp2, y, 6, MPFR_RNDN);
        mpfr_div(a[3], temp1, temp2, MPFR_RNDN);

        mpfr_set(y, a[3], MPFR_RNDN);


        mpfr_sub(z, xx, x[i], MPFR_RNDN);

        for (i = 2; i >= 0; --i) {
            mpfr_mul(y, y, z, MPFR_RNDN);
            mpfr_add(y, y, a[i], MPFR_RNDN);
        }

        mpfr_set(result, y, MPFR_RNDN);
    }

    cout << mpfr_get_d(result, MPFR_RNDN) << endl;

    // Free allocated memory
    for (int i = 0; i < n + 1; ++i) mpfr_clear(d[i]);
    for (int i = 0; i < n - 1; ++i) {
        mpfr_clear(b[i]);
    }

    for (int i = 0; i < n; ++i) {
        mpfr_clear(c[i]);
    }


    mpfr_clear(u);
    mpfr_clear(y);
    mpfr_clear(z);
    for (int i = 0; i < 4; ++i) mpfr_clear(a[i]);

    return;
}



// #include <vector>
// #include <stdexcept>
// #include <algorithm>

// using namespace std;

// double naturalsplinevalue(int n, vector<double>& x, vector<double>& f, double xx, int& st) {
//     if (n < 1) {
//         st = 1;
//         return 0.0;
//     }
//     if (xx < x[0] || xx > x[n]) {   
//         st = 3;
//         return 0.0;
//     }

//     st = 0;
//     for (int i = 0; i < n; ++i) {
//         for (int k = i + 1; k <= n; ++k) {
//             if (x[i] == x[k]) {
//                 st = 2;
//                 return 0.0;
//             }
//         }
//     }

//     vector<double> d(n + 1, 0.0);
//     vector<double> b(n - 1, 0.0);
//     vector<double> c(n, 0.0);

//     if (n > 1) {
//         double z = x[2];
//         double y = z - x[1];
//         z -= x[0];
//         double u = f[1];
//         b[0] = y / z;
//         d[1] = 6 * ((f[2] - u) / y - (u - f[0]) / (x[1] - x[0])) / z;

//         z = x[n - 2];
//         y = x[n - 1] - z;
//         z = x[n] - z;
//         u = f[n - 1];
//         c[n - 1] = y / z;
//         d[n - 1] = 6 * ((f[n] - u) / (x[n] - x[n - 1]) - (u - f[n - 2]) / y) / z;

//         for (int i = 2; i < n - 1; ++i) {
//             z = x[i];
//             y = x[i + 1] - z;
//             z -= x[i - 1];
//             u = f[i];
//             b[i - 1] = y / (y + z);
//             c[i] = 1 - b[i - 1];
//             d[i] = 6 * ((f[i + 1] - u) / y - (u - f[i - 1]) / z) / (y + z);
//         }

//         u = 2.0;
//         y = d[1] / u;
//         d[1] = y;
//         for (int i = 1; i < n - 1; ++i) {
//             double z = b[i - 1] / u;
//             b[i - 1] = z;
//             u = 2.0 - z * c[i];
//             y = (d[i + 1] - y * c[i]) / u;
//             d[i + 1] = y;
//         }

//         u = d[n - 1];
//         for (int i = n - 2; i >= 1; --i) {
//             u = d[i] - u * b[i - 1];
//             d[i] = u;
//         }
//     }

//     int i = 0;
//     while (!(xx >= x[i] && xx <= x[i + 1])) {
//         ++i;
//     }

//     double y = x[i + 1] - x[i];
//     double z = d[i + 1];
//     double u = d[i];
//     double a0 = f[i];
//     double a1 = (f[i + 1] - f[i]) / y - (2 * u + z) * y / 6;
//     double a2 = u / 2;
//     double a3 = (z - u) / (6 * y);
//     double result = a3;
//     z = xx - x[i];
//     for (int j = 2; j >= 0; --j) {
//         result = result * z + (j == 0 ? a0 : (j == 1 ? a1 : a2));
//     }

//     return result;
// }


void naturalsplinecoeffns(int n, vector<mpfr_t>& x, vector<mpfr_t>& f, array<array<mpfr_t, 25>, 4>& a, int& st) {

    mpfr_set_default_prec(256);
    // Check if n is valid
    int i, k;
    if (n < 1) {
        st = 1;
        return;
    }

    // Check for duplicate nodes
    st = 0;
    for (i = 0; i < n; ++i) {
        for (k = i + 1; k <= n; ++k) {
            if (mpfr_cmp(x[i], x[k]) == 0) {
                st = 2;
                return;
            }
        }
    }

    if (st != 0) return;
    
    // Initialize variables

    mpfr_t u, v, y, z, xi, temp1, temp2, temp3, temp4;
    mpfr_inits2(256, u, v, y, z, xi, temp1, temp2, temp3, temp4, (mpfr_ptr) 0); // Initialize MPFR variables with 256 bits precision

    vector<mpfr_t> d(n + 1), b(n), c(n);
    for (int i = 0; i < n + 1; ++i) mpfr_init(d[i]);

    for (int i = 0; i < n; ++i) {
        mpfr_init(b[i]);
    }

    for (int i = 0; i < n; i++)
    {
        mpfr_init(c[i]);
    }


    // Calculate spline coefficients
    mpfr_set_d(d[0], 0.0, MPFR_RNDN);
    mpfr_set_d(d[n], 0.0, MPFR_RNDN);

    

    if (n > 1) {
        mpfr_set(z, x[2], MPFR_RNDN);
        
        mpfr_sub(y, z, x[1], MPFR_RNDN);

        mpfr_sub(z, z, x[0], MPFR_RNDN);

        mpfr_set(u, f[1], MPFR_RNDN);

        mpfr_div(b[1], y, z, MPFR_RNDN);

        mpfr_sub(d[1], f[2], u, MPFR_RNDN);
        mpfr_div(d[1], d[1], y, MPFR_RNDN);
        mpfr_sub(temp1, u, f[0], MPFR_RNDN);
        mpfr_sub(temp2, x[1], x[0], MPFR_RNDN);
        mpfr_div(temp1, temp1, temp2, MPFR_RNDN);
        mpfr_sub(d[1], d[1], temp1, MPFR_RNDN);
        mpfr_div(d[1], d[1], z, MPFR_RNDN);
        mpfr_mul_si(d[1], d[1], 6.0, MPFR_RNDN);

        // cout << mpfr_get_d(d[1], MPFR_RNDN) << endl;

        mpfr_set(z, x[n - 2], MPFR_RNDN);

        mpfr_sub(y, x[n - 1], z, MPFR_RNDN);

        mpfr_sub(z, x[n], z, MPFR_RNDN);

        mpfr_set(u, f[n - 1], MPFR_RNDN);

        mpfr_div(c[n - 1], y, z, MPFR_RNDN);

        mpfr_sub(d[n-1], f[n], u, MPFR_RNDN);
        mpfr_sub(temp1, x[n], x[n-1], MPFR_RNDN);
        mpfr_div(d[n-1], d[n-1], temp1, MPFR_RNDN);
        mpfr_sub(temp2, u, f[n-2], MPFR_RNDN);
        mpfr_div(temp2, temp2, y, MPFR_RNDN);
        mpfr_sub(d[n-1], d[n-1], temp2, MPFR_RNDN);
        mpfr_div(d[n-1], d[n-1], z, MPFR_RNDN);
        mpfr_mul_si(d[n-1], d[n-1], 6.0, MPFR_RNDN);

        //cout << mpfr_get_d(d[n-1], MPFR_RNDN) << endl;

        for (i = 2; i <= n - 2; ++i) {
            mpfr_set(z, x[i], MPFR_RNDN);

            mpfr_sub(y, x[i + 1], z, MPFR_RNDN);

            mpfr_sub(z, z, x[i - 1], MPFR_RNDN);

            mpfr_set(u, f[i], MPFR_RNDN);

            mpfr_add(temp1, y, z, MPFR_RNDN);
            mpfr_div(b[i], y, temp1, MPFR_RNDN);
            
            mpfr_si_sub(c[i], 1, b[i], MPFR_RNDN);

            mpfr_sub(d[i], f[i+1], u, MPFR_RNDN);
            mpfr_div(d[i], d[i], y, MPFR_RNDN);
            mpfr_sub(temp1, u, f[i-1], MPFR_RNDN);
            mpfr_div(temp1, temp1, z, MPFR_RNDN);
            mpfr_sub(d[i], d[i], temp1, MPFR_RNDN);
            mpfr_add(temp2, y, z, MPFR_RNDN);
            mpfr_div(d[i], d[i], temp2, MPFR_RNDN);
            mpfr_mul_si(d[i], d[i], 6.0, MPFR_RNDN);
        }

        mpfr_set_d(u, 2.0, MPFR_RNDN);
        
        mpfr_div(y, d[1], u, MPFR_RNDN);

        mpfr_set(d[1], y, MPFR_RNDN);

        if (n > 2) {
            for (i = 1; i <= n - 2; ++i) {
                mpfr_div(z, b[i], u, MPFR_RNDN);

                mpfr_set(b[i], z, MPFR_RNDN);

                mpfr_mul(u, z, c[i+1], MPFR_RNDN);
                mpfr_si_sub(u, 2.0, u, MPFR_RNDN);

                mpfr_mul(y, y, c[i+1], MPFR_RNDN);
                mpfr_sub(y, d[i+1], y, MPFR_RNDN);
                mpfr_div(y, y, u, MPFR_RNDN);

                mpfr_set(d[i + 1], y, MPFR_RNDN);
            }
        }

        mpfr_set(u, d[n - 1], MPFR_RNDN);
        for (i = n - 2; i >= 1; --i) {
            mpfr_mul(u, u, b[i], MPFR_RNDN);
            mpfr_sub(u, d[i], u, MPFR_RNDN);
            
            mpfr_set(d[i], u, MPFR_RNDN);
        }
    }

    for (i = 0; i < n; ++i) {
        mpfr_set(u, f[i], MPFR_RNDN);

        // cout << mpfr_get_d(u, MPFR_RNDN) << endl;

        mpfr_set(xi, x[i], MPFR_RNDN);

        mpfr_sub(z, x[i + 1], xi, MPFR_RNDN);

        // cout << mpfr_get_d(z, MPFR_RNDN) << endl;
        
        mpfr_set(y, d[i], MPFR_RNDN);

        // cout << mpfr_get_d(y, MPFR_RNDN) << endl;

        mpfr_sub(v, f[i+1], u, MPFR_RNDN);
        mpfr_div(v, v, z, MPFR_RNDN);
        mpfr_mul_si(temp1, y, 2.0, MPFR_RNDN);
        mpfr_add(temp1, temp1, d[i+1], MPFR_RNDN);
        mpfr_mul(temp1, temp1, z, MPFR_RNDN);
        mpfr_div_si(temp1, temp1, 6.0, MPFR_RNDN);
        mpfr_sub(v, v, temp1, MPFR_RNDN);
        
        // cout << mpfr_get_d(d[i+1], MPFR_RNDN) << endl;

        mpfr_sub(temp1, d[i+1], y, MPFR_RNDN);
        mpfr_mul_si(temp2, z, 6, MPFR_RNDN);
        mpfr_div(z, temp1, temp2, MPFR_RNDN);

        // cout << mpfr_get_d(z, MPFR_RNDN) << endl;

        mpfr_div_si(y, y, 2, MPFR_RNDN);

        // cout << mpfr_get_d(z, MPFR_RNDN) << endl;

        mpfr_mul_si(a[0][i], z, -1, MPFR_RNDN);
        mpfr_mul(a[0][i], a[0][i], xi, MPFR_RNDN);
        mpfr_add(a[0][i], a[0][i], y, MPFR_RNDN);
        mpfr_mul(a[0][i], a[0][i], xi, MPFR_RNDN);
        mpfr_sub(a[0][i], a[0][i], v, MPFR_RNDN);
        mpfr_mul(a[0][i], a[0][i], xi, MPFR_RNDN);
        mpfr_add(a[0][i], a[0][i], u, MPFR_RNDN);

        mpfr_mul_si(u, z, 3, MPFR_RNDN);
        mpfr_mul(u, u, xi, MPFR_RNDN);

        mpfr_mul_si(a[1][i], y, 2, MPFR_RNDN);
        mpfr_sub(a[1][i], u, a[1][i], MPFR_RNDN);
        mpfr_mul(a[1][i], a[1][i], xi, MPFR_RNDN);
        mpfr_add(a[1][i], a[1][i], v, MPFR_RNDN);

        mpfr_sub(a[2][i], y, u, MPFR_RNDN);
        mpfr_set(a[3][i], z, MPFR_RNDN);

        // cout << mpfr_get_d(a[0][0], MPFR_RNDN) << endl;
        // cout << mpfr_get_d(a[1][0], MPFR_RNDN) << endl;
        // cout << mpfr_get_d(a[2][0], MPFR_RNDN) << endl;
        // cout << mpfr_get_d(a[3][0], MPFR_RNDN) << endl;
    }

    // Clear MPFR variables
    mpfr_clears(u, v, y, z, xi, temp1, temp2, temp3, temp4, (mpfr_ptr) 0);

    for (int i = 0; i < n + 1; ++i) mpfr_clear(d[i]);

    for (int i = 0; i < n - 1; ++i) {
        mpfr_clear(b[i]);
    }

    for (int i = 0; i < n; ++i) {
        mpfr_clear(c[i]);
    }

    // for (i = 0; i < 4; ++i) {
    //     for (k = 0; k < n; ++k) {
    //         mpfr_clear(a[i][k]);
    //     }
    // }
}

void coeff_float_to_interval(vector<double> x, vector<double> f, vector<Interval<mpreal>>& interval_x, vector<Interval<mpreal>>& interval_f)
{
    mpreal::set_default_prec(256);
    for (int i = 0; i < (int) interval_x.size(); i++)
    {   
        string x_double = to_string(x[i]);
        string f_double = to_string(f[i]);

        interval_x[i].a = LeftRead<mpreal>(x_double);
        interval_x[i].b = RightRead<mpreal>(x_double);

        interval_f[i].a = LeftRead<mpreal>(f_double);
        interval_f[i].b = RightRead<mpreal>(f_double);
    }
}


void interval_naturalsplinecoeffns(int n, vector<Interval<mpreal>>& x, vector<Interval<mpreal>>& f, array<array<Interval<mpreal>, 25>, 4>& a, int& st) {
    mpreal::set_default_prec(256);
    
    // Check if n is valid
    if (n < 1) {
        st = 1;
        return;
    }

    // Check for duplicate nodes
    st = 0;
    for (int i = 0; i < n; ++i) {
        for (int k = i + 1; k <= n; ++k) {
            if (x[i].a == x[k].a && x[i].b == x[k].b) {
                st = 2;
                return;
            }
        }
    }

    if (st != 0) return;
    
    // Initialize variables
    Interval<mpreal> u, v, y, z, xi, temp1, temp2, temp3, temp4;

    vector<Interval<mpreal>> d(n + 1), b(n), c(n);

    // Calculate spline coefficients
    d[0] = {0.0, 0.0};
    d[n] = {0.0, 0.0};

    if (n > 1) {
        z = x[2];
        y = ISub(x[2], x[1]);
        z = ISub(x[2], x[0]);
        u = f[1];
        b[1] = IDiv(y, z);

        d[1] = IMul(IDiv(ISub(IDiv(ISub(f[2], u), y), IDiv(ISub(u, f[0]), ISub(x[1], x[0]))), z), Interval<mpreal>(6.0, 6.0));

        z = x[n-2];
        y = ISub(x[n-1], z);
        z = ISub(x[n], z);
        u = f[n-1];
        c[n-1] = IDiv(y, z);

        d[n-1] = IMul(IDiv(ISub(IDiv(ISub(f[n], u), ISub(x[n], x[n-1])), IDiv(ISub(u, f[n-2]), y)), z), Interval<mpreal>(6.0, 6.0));

        for (int i = 2; i <= n - 2; ++i) {
            z = x[i];
            y = ISub(x[i + 1], z);
            z = ISub(z, x[i - 1]);
            u = f[i];

            b[i] = IDiv(y, IAdd(y, z));
            c[i] = ISub(Interval<mpreal>(1.0, 1.0), b[i]);

            d[i] = IMul(IDiv(ISub(IDiv(ISub(f[i+1],u), y), IDiv(ISub(u, f[i-1]), z)), IAdd(y, z)), Interval<mpreal>(6.0, 6.0));
        }

        u = Interval<mpreal>(2.0, 2.0);
        y = IDiv(d[1], u);
        d[1] = y;

        if (n > 2) {
            for (int i = 1; i <= n - 2; ++i) {
                z = IDiv(b[i], u);
                b[i] = z;

                u = ISub(Interval<mpreal>(2.0, 2.0), IMul(z, c[i+1]));

                y = IDiv(ISub(d[i+1], IMul(y, c[i+1])), u);

                d[i + 1] = y;
            }
        }

        u = d[n - 1];

        for (int i = n - 2; i >= 1; --i) {
            u = ISub(d[i], IMul(u, b[i]));

            d[i] = u;
        }
    }

    for (int i = 0; i < n; ++i) {
        u = f[i];
        xi = x[i];
        z = ISub(x[i + 1], xi);
        y = d[i];

        v = ISub(IDiv(ISub(f[i+1], u), z), IDiv(IMul(IAdd(IMul(Interval<mpreal>(2.0, 2.0), y), d[i+1]), z), Interval<mpreal>(6.0, 6.0)));

        z = IDiv(ISub(d[i+1], y), IMul(Interval<mpreal>(6.0, 6.0), z));

        y = IDiv(y, Interval<mpreal>(2.0, 2.0));

        a[0][i] = IAdd(IMul(ISub(IMul(IAdd(IMul(IMul(Interval<mpreal>(-1.0, -1.0), z), xi), y), xi), v), xi), u);

        u = IMul(IMul(Interval<mpreal>(3.0, 3.0), z), xi);

        a[1][i] = IAdd(IMul(ISub(u, IMul(Interval<mpreal>(2.0, 2.0), y)), xi), v);

        a[2][i] = ISub(y, u);

        a[3][i] = z;
    }
}
