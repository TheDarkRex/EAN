#pragma once

#define MPFR_USE_NO_MACRO
#define MPFR_USE_INTMAX_T

#include <Interval.h>
#include <vector>

using namespace interval_arithmetic;
using namespace std;

double naturalsplinevalue(int n, vector<double>& x, vector<double>& f, double xx, int& st);