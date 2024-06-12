#pragma once

#define MPFR_USE_NO_MACRO
#define MPFR_USE_INTMAX_T

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "./naturalspline.h"


#include <stdio.h>
#include <vector>
#include <string>
#include <Interval.h>
#include <mpfr.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>



#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h> // Will drag system OpenGL headers


class App{
public:
    App(); 
    int init(); 
    int run();
    int update();
    int term();


private: 
    GLFWwindow* window;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool interval = false;
    bool float_point = false;
    bool float_point_to_interval = false;
    bool factorGUI = false;
    bool valueGUI = false;

    // vector<mpfr_t> mpfr_vector_x;
    // vector<mpfr_t> mpfr_vector_f;

    mpfr_t mpfr_xx;
    mpfr_t mpfr_result;

    double xx;

    string left, right;

    Interval<double> interval_xx;
    Interval<mpreal> interval_mp_xx;

    Interval<double> interval_result;
    Interval<mpreal> interval_mp_result;

    vector<Interval<double>> interval_x = {{1.0, 1.0}};
    vector<Interval<double>> interval_f = {{1.0, 1.0}};

    int n = 0;
    int st;
    // Interval<double> i = {1.0, 1.0}
    vector<double> x = {1.0};
    vector<double> f = {1.0};


    // std::vector<Interval<double>> xI;

    double result; 
    

}; 