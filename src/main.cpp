
// #define MPFR_USE_NO_MACRO
// #define MPFR_USE_INTMAX_T
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
// #include <stdio.h>
// #include <vector>
// #include <string>
// #include <Interval.h>
// #include <mpfr.h>
// #include "./naturalspline.h"
// #define GL_SILENCE_DEPRECATION
// #if defined(IMGUI_IMPL_OPENGL_ES2)
// #include <GLES2/gl2.h>
// #endif
// #include <GLFW/glfw3.h> // Will drag system OpenGL headers

// // [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// // To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// // Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
// #if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
// #pragma comment(lib, "legacy_stdio_definitions")
// #endif

// // This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
// #ifdef __EMSCRIPTEN__
// #include "../libs/emscripten/emscripten_mainloop_stub.h"
// #endif

// static void glfw_error_callback(int error, const char* description)
// {
//     fprintf(stderr, "GLFW Error %d: %s\n", error, description);
// }

// // Main code
// int main(int, char**)
// {
//     glfwSetErrorCallback(glfw_error_callback);
//     if (!glfwInit())
//         return 1;

//     // Decide GL+GLSL versions
// #if defined(IMGUI_IMPL_OPENGL_ES2)
//     // GL ES 2.0 + GLSL 100
//     const char* glsl_version = "#version 100";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//     glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
// #elif defined(__APPLE__)
//     // GL 3.2 + GLSL 150
//     const char* glsl_version = "#version 150";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
// #else
//     // GL 3.0 + GLSL 130
//     const char* glsl_version = "#version 130";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//     //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//     //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
// #endif

//     // Create window with graphics context
//     GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
//     if (window == nullptr)
//         return 1;
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1); // Enable vsync

//     // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

//     // Setup Dear ImGui style
//     ImGui::StyleColorsDark();
//     //ImGui::StyleColorsLight();

//     // Setup Platform/Renderer backends
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
// #ifdef __EMSCRIPTEN__
//     ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
// #endif
//     ImGui_ImplOpenGL3_Init(glsl_version);

//     // Load Fonts
//     // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//     // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//     // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//     // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//     // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
//     // - Read 'docs/FONTS.md' for more instructions and details.
//     // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//     // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
//     //io.Fonts->AddFontDefault();
//     //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
//     //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//     //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//     //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//     //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
//     //IM_ASSERT(font != nullptr);

//     // Our state
//     bool show_ean_window = true;
//     ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//     bool interval = false;
//     bool float_point = false;
//     bool float_point_to_interval = false;
//     bool show_result = false;
//     bool show_test = false;
//     bool test_calc = false;

//     static vector<mpfr_t> mpfr_vector_x(100);
//     static vector<mpfr_t> mpfr_vector_f(100);

//     static mpfr_t mpfr_xx;
//     static mpfr_t mpfr_result;

//     mpfr_init2(mpfr_xx, 256);
//     mpfr_init2(mpfr_result, 256);

//     double result;

//     // Main loop
// #ifdef __EMSCRIPTEN__
//     // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
//     // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
//     io.IniFilename = nullptr;
//     EMSCRIPTEN_MAINLOOP_BEGIN
// #else
//     while (!glfwWindowShouldClose(window))
// #endif
//     {
//         // Poll and handle events (inputs, window resize, etc.)
//         // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
//         // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
//         // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
//         // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
//         glfwPollEvents();

//         // Start the Dear ImGui frame
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();
        
//         int width, height;
//         glfwGetWindowSize(window, &width, &height);

//         ImGui::SetNextWindowSize(ImVec2(width, height)); // ensures ImGui fits the GLFW window

//         ImGui::SetNextWindowPos(ImVec2(0, 0));

//         if (show_ean_window)
//         {
//             static double xx;
//             static int n;
//             static int st;
//             // Interval<double> i = {1.0, 1.0}
//             static std::vector<double> x = {1.0};
//             static std::vector<double> f = {1.0};
//             // std::vector<Interval<double>> xI;

//             ImGui::Begin("EAN App Window", &show_ean_window);
            

//             if (ImGui::Checkbox("Calculations in interval values", &interval))
//             {
//                 interval = true;
//                 float_point = false;
//                 float_point_to_interval = false;
//             }

//             if (ImGui::Checkbox("Calculations in float point values", &float_point))
//             {
//                 interval = false;
//                 float_point = true;
//                 float_point_to_interval = false;
//             }

//             if (ImGui::Checkbox("Calculations from float point to interval values", &float_point_to_interval))
//             {
//                 interval = false;
//                 float_point = false;
//                 float_point_to_interval = true;
//             }
            
//             // Gathering data
//             if (interval)
//             {
//                 ImGui::InputDouble("Point:", &xx);
//                 ImGui::InputInt("Number of nodes:", &n);
//                 ImGui::Text("Point: %lf, Number of nodes: %d", xx, n);
//             }


//             if (float_point_to_interval)
//             {
//                 ImGui::InputDouble("Point:", &xx);
                
//                 ImGui::InputInt("Number of nodes:", &n);
//                 ImGui::Text("Point: %lf, Number of nodes: %d", xx, n);
//             }
            
            
//             if (float_point)
//             {
//                 ImGui::SetNextItemWidth(100);
//                 ImGui::InputDouble("Point:", &xx);
//                 ImGui::SetNextItemWidth(100);
//                 ImGui::InputInt("Number of nodes:", &n);
                
//                 if (n < (int) x.size())
//                 {
//                     x.push_back(1.0);
//                     f.push_back(1.0);
//                 }

//                 if (n > (int) x.size())
//                 {
//                     x.pop_back();
//                     f.pop_back();
//                 }
                
//                 ImGui::Text("Point: %lf, Number of nodes: %d", xx, n);
//             }


//             if (float_point)
//             {

//                 for (int i = 0; i <= n; i++)
//                 {   
//                     std::string x_title = "x["+std::to_string(i)+"]";
//                     std::string f_title = "f["+std::to_string(i)+"]"; 

//                     ImGui::SetNextItemWidth((int)(100));
//                     ImGui::InputDouble(x_title.c_str(), &x[i]);
//                     mpfr_init2(mpfr_vector_x[i], 256);
//                     mpfr_set(mpfr_vector_x[i], x[i], MPFR_RNDN);

//                     ImGui::SameLine();

//                     ImGui::SetNextItemWidth((int)(100));
//                     ImGui::InputDouble(f_title.c_str(), &f[i]);
//                     mpfr_init2(mpfr_vector_f[i], 256);
//                     mpfr_set(mpfr_vector_f[i], f[i], MPFR_RNDN);
//                 }
//             }
            
//             // Calculating
//             if (ImGui::Button("Calculate"))
//             {
//                 show_result = true;
//             }

//             if (show_result)
//             {
//                 ImGui::Begin("Result");
//                 ImGui::Text("Test");

//                 ImGui::Text("Point: %lf", xx);
//                 ImGui::Text("Number of nodes: %d", n);

//                 for (int i = 0; i <= n; i++)
//                 {

//                     ImGui::Text("x[%d] = %lf", i, mpfr_get_d(mpfr_vector_x[i], MPFR_RNDN));
//                     ImGui::SameLine();
//                     ImGui::Text("f[%d] = %lf", i, mpfr_get_d(mpfr_vector_f[i], MPFR_RNDN));
//                 }

//                 mpfr_set(mpfr_xx, xx, MPFR_RNDN);

//                 // cout << mpfr_get_d(mpfr_xx, MPFR_RNDN) << endl;
//                 // result = naturalsplinevalue(n, x, f, xx, st);
//                 // naturalsplinevalue(n, mpfr_vector_x, mpfr_vector_f, mpfr_xx, mpfr_result, st);
                
//                 naturalsplinevalue(n, mpfr_vector_x, mpfr_vector_f, mpfr_xx, mpfr_result, st);

//                 if (st == 0)
//                 {
//                     ImGui::Text("Result: %.10e St: %d", mpfr_get_d(mpfr_result, MPFR_RNDN), st);
//                 }
//                 else
//                 {
//                     ImGui::Text("St: %d", st);
//                 }
                

//                 if (ImGui::Button("Close"))
//                 {
//                     show_result = false;
//                 }
                
//                 ImGui::End();
//             }

//             ImGui::End();
//         }
        

//         // Rendering
//         ImGui::Render();
//         int display_w, display_h;
//         glfwGetFramebufferSize(window, &display_w, &display_h);
//         glViewport(0, 0, display_w, display_h);
//         glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//         glfwSwapBuffers(window);
//     }
// #ifdef __EMSCRIPTEN__
//     EMSCRIPTEN_MAINLOOP_END;
// #endif

//     // Cleanup
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();

//     glfwDestroyWindow(window);
//     glfwTerminate();

//     return 0;
// }


#include "app.h"

#define MPFR_USE_NO_MACRO
#define MPFR_USE_INTMAX_T

int main() {
	App app;
	if(app.init())
		return 1; 
	app.run(); 
	app.term();  
	return 0;
}