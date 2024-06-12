#pragma once

#define MPFR_USE_NO_MACRO
#define MPFR_USE_INTMAX_T

#include <Interval.h>
#include <mpfr.h>
#include <vector>

using namespace interval_arithmetic;
using namespace std;

void naturalsplinevalue(int n, vector<mpfr_t>& x, vector<mpfr_t>& f, mpfr_t& xx, mpfr_t& result, int& st);
void interval_naturalsplinevalue(int n, vector<Interval<mpreal>>& x, vector<Interval<mpreal>>& f, Interval<mpreal>& xx, Interval<mpreal>& result, int& st);
void float_to_interval(double xx, vector<double> x, vector<double> f, vector<Interval<mpreal>>& interval_x, vector<Interval<mpreal>>& interval_f, Interval<mpreal>& interval_xx);

void naturalsplinecoeffns(int n, vector<mpfr_t>& x, vector<mpfr_t>& f, array<array<mpfr_t, 25>, 4>& a, int& st);
void interval_naturalsplinecoeffns(int n, vector<Interval<mpreal>>& x, vector<Interval<mpreal>>& f, array<array<Interval<mpreal>, 25>, 4>& a, int& st);
void coeff_float_to_interval(vector<double> x, vector<double> f, vector<Interval<mpreal>>& interval_x, vector<Interval<mpreal>>& interval_f);

// double naturalsplinevalue(int n, vector<double>& x, vector<double>& f, double xx, int& st);