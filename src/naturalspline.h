#pragma once

#define MPFR_USE_NO_MACRO
#define MPFR_USE_INTMAX_T

#include <Interval.h>
#include <vector>

using namespace interval_arithmetic;

double naturalsplinevalue(int n, std::vector<double> x, std::vector<double> f, double xx, int *st);