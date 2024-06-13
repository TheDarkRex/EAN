#include "./app.h"

#define MPFR_USE_NO_MACRO
#define MPFR_USE_INTMAX_T

vector<mpfr_t> mpfr_vector_f(25);
vector<mpfr_t> mpfr_vector_x(25);

vector<Interval<mpreal>> interval_mp_x(25);
vector<Interval<mpreal>> interval_mp_f(25);

//vector<vector<mpfr_t>> a_mpfr(4, vector<mpfr_t>(25));

//mpfr_t a_mpfr[4][25];
array<array<mpfr_t, 25>, 4> a_mpfr;

array<array<Interval<mpreal>, 25>, 4> a_mp;

App::App(){
    bool interval = false;
    bool float_point = false;
    bool float_point_to_interval = false;
    bool factorGUI = false;
    bool valueGUI = false;


    mpfr_t mpfr_xx;
    mpfr_t mpfr_result;

    double xx;

    Interval<double> interval_xx = {1.0, 1.0};

    Interval<mpreal> interval_mp_xx;
    Interval<mpreal> interval_mp_result;

    vector<Interval<double>> inteval_x = {{1.0, 1.0}};
    vector<Interval<double>> inteval_f = {{1.0, 1.0}};

    int n = 0;
    int st;
    // Interval<double> i = {1.0, 1.0}
    vector<double> x = {1.0};
    vector<double> f = {1.0};
    // std::vector<Interval<double>> xI;

    double result; 
}

int App::init(){
    mpreal::set_default_prec(256);

    mpfr_init(mpfr_xx);
    mpfr_init(mpfr_result);

    for (int i = 0; i < mpfr_vector_x.size(); i++)
    {
        mpfr_init(mpfr_vector_x[i]);
        mpfr_init(mpfr_vector_f[i]);
    }

    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 25; ++k) {
            mpfr_init(a_mpfr[i][k]);
        }
    }

    st = -1;

    //

    // vector<double> x(7);
    // vector<double> f(7);

    // n = 6;
    // xx = 23.5;

    // x[0] = 17;
    // x[1] = 20;
    // x[2] = 23;
    // x[3] = 24;
    // x[4] = 25;
    // x[5] = 27;
    // x[6] = 27.7;

    // f[0] = 4.5;
    // f[1] = 7;
    // f[2] = 6.1;
    // f[3] = 5.6;
    // f[4] = 5.8;
    // f[5] = 5.2;
    // f[6] = 4.1;

    // mpfr_set(mpfr_vector_x[0], 17, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[1], 20, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[2], 23, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[3], 24, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[4], 25, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[5], 27, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[6], 27.7, MPFR_RNDN);

    // mpfr_set(mpfr_vector_x[0], 4.5, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[1], 7, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[2], 6.1, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[3], 5.6, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[4], 5.8, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[5], 5.2, MPFR_RNDN);
    // mpfr_set(mpfr_vector_x[6], 4.1, MPFR_RNDN);
    //

	if (!glfwInit())
		return 1;

    // Create window with graphics context
	this->window = glfwCreateWindow(1280, 720, "Natural Spline", nullptr, nullptr);
	if (window == nullptr)
		return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
    return 0; 
}

int App::run(){

    while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		update(); // gui of the app 

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glfwSwapBuffers(window);
	}
	return 0; 
}

int App::update(){
    // cout << mpfr_vector_x.size() << endl; 
    // Start the Dear ImGui frame
    int width, height;
    glfwGetWindowSize(window, &width, &height);


	ImGui::SetNextWindowPos(ImVec2(0, height/2));
	ImGui::SetNextWindowSize(ImVec2(width, height/2));
	ImGui::Begin("Output", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Output:");

    if (valueGUI)
    {
        if (st == 0)
        {   
            if (float_point)
            {
                ImGui::Text("Point: %lf", xx);
                ImGui::Text("Number of nodes: %d", n);

                for (int i = 0; i <= n; i++)
                {
                    ImGui::Text("x[%d] = %lf", i, mpfr_get_d(mpfr_vector_x[i], MPFR_RNDN));
                    ImGui::SameLine();
                    ImGui::Text("f[%d] = %lf", i, mpfr_get_d(mpfr_vector_f[i], MPFR_RNDN));
                }

                ImGui::Text("Result: %.15e St: %d", mpfr_get_d(mpfr_result, MPFR_RNDN), st);
            }
            
            if (interval)
            {
                interval_mp_xx.IEndsToStrings(left, right);

                char * leftChar = new char[left.length()+1];
                strcpy(leftChar, left.c_str());

                char * rightChar = new char[right.length()+1];
                strcpy(rightChar, right.c_str());

                ImGui::Text("Point: [%s, %s]", leftChar, rightChar);
                ImGui::Text("Number of nodes: %d", n);

                for (int i = 0; i <= n; i++)
                {
                    
                    interval_mp_x[i].IEndsToStrings(left, right);

                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());


                    ImGui::Text("x[%d] = [%s, %s]", i, leftChar, rightChar);
                    ImGui::SameLine();


                    interval_mp_f[i].IEndsToStrings(left, right);

                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());

                    ImGui::Text("f[%d] = [%s, %s]", i, leftChar, rightChar);
                }

                interval_mp_result.IEndsToStrings(left, right);

                leftChar = new char[left.length()+1];
                strcpy(leftChar, left.c_str());

                rightChar = new char[right.length()+1];
                strcpy(rightChar, right.c_str());

                resultWidth = interval_mp_result.GetWidth().toDouble();

                ImGui::Text("Result: [%s, %s] Width: %.15e St: %d", leftChar, rightChar, resultWidth, st);
            }

            if (float_point_to_interval)
            {
                interval_mp_xx.IEndsToStrings(left, right);

                char * leftChar = new char[left.length()+1];
                strcpy(leftChar, left.c_str());

                char * rightChar = new char[right.length()+1];
                strcpy(rightChar, right.c_str());

                ImGui::Text("Point: [%s, %s]", leftChar, rightChar);
                ImGui::Text("Number of nodes: %d", n);


                for (int i = 0; i <= n; i++)
                {
                    interval_mp_x[i].IEndsToStrings(left, right);
                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());

                    ImGui::Text("x[%d] = [%s, %s]", i, leftChar, rightChar);
                    ImGui::SameLine();

                    interval_mp_f[i].IEndsToStrings(left, right);
                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());

                    ImGui::Text("f[%d] = [%s, %s]", i, leftChar, rightChar);
                }

                interval_mp_result.IEndsToStrings(left, right);

                leftChar = new char[left.length()+1];
                strcpy(leftChar, left.c_str());

                rightChar = new char[right.length()+1];
                strcpy(rightChar, right.c_str());

                resultWidth = interval_mp_result.GetWidth().toDouble();

                ImGui::Text("Result: [%s, %s] Width: %.15e St: %d", leftChar, rightChar, resultWidth, st);
            }
            
            
        }
        if(st != 0 and st > 0)
        {
            ImGui::Text("St: %d", st);
        }
    }
    
    if (factorGUI)
    {   
        if (st == 0)
        {
            if (float_point)
            {
                ImGui::Text("Point: %lf", xx);
                ImGui::Text("Number of nodes: %d", n);

                for (int i = 0; i <= n; i++)
                {
                    ImGui::Text("x[%d] = %lf", i, mpfr_get_d(mpfr_vector_x[i], MPFR_RNDN));
                    ImGui::SameLine();
                    ImGui::Text("f[%d] = %lf", i, mpfr_get_d(mpfr_vector_f[i], MPFR_RNDN));
                }

                ImGui::Text("Result: St: %d", st);

                // cout << a_mpfr.size() << endl;
                
        // cout << mpfr_get_d(a_mpfr[0][0], MPFR_RNDN) << endl;
        // cout << mpfr_get_d(a_mpfr[1][0], MPFR_RNDN) << endl;
        // cout << mpfr_get_d(a_mpfr[2][0], MPFR_RNDN) << endl;
        // cout << mpfr_get_d(a_mpfr[3][0], MPFR_RNDN) << endl;

                for (int i = 0; i < n; i++)
                {
                    for (int k = 0; k < 4; k++)
                    {   
                        ImGui::Text("a[%d, %d] = %.15e", k, i, mpfr_get_d(a_mpfr[k][i], MPFR_RNDN));
                        //ImGui::Text("test");
                    }
                    
                }
            }

            if (interval)
            {
                char * leftChar;
                char * rightChar;
                ImGui::Text("Number of nodes: %d", n);

                for (int i = 0; i <= n; i++)
                {
                    
                    interval_mp_x[i].IEndsToStrings(left, right);

                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());


                    ImGui::Text("x[%d] = [%s, %s]", i, leftChar, rightChar);
                    ImGui::SameLine();


                    interval_mp_f[i].IEndsToStrings(left, right);

                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());

                    ImGui::Text("f[%d] = [%s, %s]", i, leftChar, rightChar);
                }

                for (int i = 0; i < n; i++)
                {
                    for (int k = 0; k < 4; k++)
                    {   
                        a_mp[k][i].IEndsToStrings(left, right);

                        leftChar = new char[left.length()+1];
                        strcpy(leftChar, left.c_str());

                        rightChar = new char[right.length()+1];
                        strcpy(rightChar, right.c_str());

                        resultWidth = a_mp[k][i].GetWidth().toDouble();

                        ImGui::Text("a[%d, %d] = [%s, %s] Width: %.15e", k, i, leftChar, rightChar, resultWidth);
                        //ImGui::Text("test");
                    }
                    
                }

                ImGui::Text("St: %d", st);
            }

            if (float_point_to_interval)
            {

                char * leftChar;
                char * rightChar;


                ImGui::Text("Number of nodes: %d", n);


                for (int i = 0; i <= n; i++)
                {
                    interval_mp_x[i].IEndsToStrings(left, right);
                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());

                    ImGui::Text("x[%d] = [%s, %s]", i, leftChar, rightChar);
                    ImGui::SameLine();

                    interval_mp_f[i].IEndsToStrings(left, right);
                    leftChar = new char[left.length()+1];
                    strcpy(leftChar, left.c_str());

                    rightChar = new char[right.length()+1];
                    strcpy(rightChar, right.c_str());

                    ImGui::Text("f[%d] = [%s, %s]", i, leftChar, rightChar);
                }

                for (int i = 0; i < n; i++)
                {
                    for (int k = 0; k < 4; k++)
                    {   
                        a_mp[k][i].IEndsToStrings(left, right);

                        leftChar = new char[left.length()+1];
                        strcpy(leftChar, left.c_str());

                        rightChar = new char[right.length()+1];
                        strcpy(rightChar, right.c_str());

                        resultWidth = a_mp[k][i].GetWidth().toDouble();

                        ImGui::Text("a[%d, %d] = [%s, %s] Width: %.15e", k, i, leftChar, rightChar, resultWidth);
                        //ImGui::Text("test");
                    }
                    
                }

                ImGui::Text("St: %d", st);
            }
        }
        if(st != 0 and st > 0)
        {
            ImGui::Text("St: %d", st);
        }
    }
    
    
	ImGui::End(); 

    ImGui::SetNextWindowSize(ImVec2(width, height/2)); // ensures ImGui fits the GLFW window
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("EAN App Window", NULL);

    if(ImGui::Checkbox("Value", &valueGUI)){
        valueGUI = true;
        factorGUI = false;
    }

    ImGui::SameLine();

    if (ImGui::Checkbox("Coeffns", &factorGUI))
    {
        valueGUI = false;
        factorGUI = true;
    }
    

    if (valueGUI)
    {

        if (ImGui::Checkbox("Calculations in interval values", &interval)) {
            interval = true;
            float_point = false;
            float_point_to_interval = false;
        }

        if (ImGui::Checkbox("Calculations in float point values", &float_point)) {
            interval = false;
            float_point = true;
            float_point_to_interval = false;
        }

        if (ImGui::Checkbox("Calculations from float point to interval values", &float_point_to_interval)) {
            interval = false;
            float_point = false;
            float_point_to_interval = true;
        }


        //interval
        if (interval) {
            ImGui::SetNextItemWidth(100);
            ImGui::InputDouble("Point left", &interval_xx.a);

            string ixxa = to_string(interval_xx.a);
            interval_mp_xx.a = LeftRead<mpreal>(ixxa);

            ImGui::SetNextItemWidth(100);

            ImGui::SameLine();

            ImGui::InputDouble("Point right", &interval_xx.b);


            string ixxb = to_string(interval_xx.b);
            interval_mp_xx.b = LeftRead<mpreal>(ixxb);

            ImGui::SetNextItemWidth(100);

            
            if(ImGui::InputInt("Number of nodes:", &n)){
                if (n < 0)
                {
                    n = 0;
                }
            
                interval_x.resize(n+1, {1.0, 1.0}); // resize with default value 1.0
                interval_f.resize(n+1, {1.0, 1.0}); // resize with default value 1.0
            }

            ImGui::Text("Point: [%lf, %lf], Number of nodes: %d", interval_xx.a, interval_xx.b, n);

            for (int i = 0; i < (int) interval_x.size(); i++)
            {   
                std::string x_left = "left x["+std::to_string(i)+"]";
                std::string x_title = "right x["+std::to_string(i)+"] ||";

                std::string f_left = "left f["+std::to_string(i)+"]";
                std::string f_title = "right f["+std::to_string(i)+"]"; 

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_left.c_str(), &interval_x[i].a);
                string ixa = to_string(interval_x[i].a);
                interval_mp_x[i].a = LeftRead<mpreal>(ixa);

                ImGui::SameLine();


                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_title.c_str(), &interval_x[i].b);
                string ixb = to_string(interval_x[i].b);
                interval_mp_x[i].b = LeftRead<mpreal>(ixb);

                ImGui::SameLine();

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_left.c_str(), &interval_f[i].a);
                string ifa = to_string(interval_f[i].a);
                interval_mp_f[i].a = LeftRead<mpreal>(ifa);

                ImGui::SameLine();

                
                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_title.c_str(), &interval_f[i].b);
                string ifb = to_string(interval_f[i].b);
                interval_mp_f[i].b = LeftRead<mpreal>(ifb);
            }
        }


        // float point to interval
        if (float_point_to_interval) {
            ImGui::SetNextItemWidth(100);
            ImGui::InputDouble("Point:", &xx);
            ImGui::SetNextItemWidth(100);

            if(ImGui::InputInt("Number of nodes:", &n)){
                if (n < 0)
                {
                    n = 0;
                }
                
                x.resize(n+1, 1.0); // resize with default value 1.0
                f.resize(n+1, 1.0); // resize with default value 1.0
            }

            ImGui::Text("Point: %lf, Number of nodes: %d", xx, n);

            for (int i = 0; i < (int) x.size(); i++)
            {   
                std::string x_title = "x["+std::to_string(i)+"]";
                std::string f_title = "f["+std::to_string(i)+"]"; 
                // cout << x.size() << endl;
                // cout << i << endl;
                // cout << x[i] << endl;
                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_title.c_str(), &x[i]);

                ImGui::SameLine();

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_title.c_str(), &f[i]);
            }
        }

        // float point
        if (float_point)
        {
            ImGui::SetNextItemWidth(100);
            ImGui::InputDouble("Point:", &xx);
            ImGui::SetNextItemWidth(100);
            if(ImGui::InputInt("Number of nodes:", &n)){
                if (n < 0)
                {
                    n = 0;
                }
                
                x.resize(n+1, 1.0); // resize with default value 1.0
                f.resize(n+1, 1.0); // resize with default value 1.0
            }

        // vector<double> x(7);
        // vector<double> f(7);

        // x[0] = 17;
        // x[1] = 20;
        // x[2] = 23;
        // x[3] = 24;
        // x[4] = 25;
        // x[5] = 27;
        // x[6] = 27.7;

        // f[0] = 4.5;
        // f[1] = 7;
        // f[2] = 6.1;
        // f[3] = 5.6;
        // f[4] = 5.8;
        // f[5] = 5.2;
        // f[6] = 4.1;
        
            ImGui::Text("Point: %lf, Number of nodes: %d", xx, n);

            for (int i = 0; i < (int) x.size(); i++)
            {   
                std::string x_title = "x["+std::to_string(i)+"]";
                std::string f_title = "f["+std::to_string(i)+"]"; 
                // cout << x.size() << endl;
                // cout << i << endl;
                // cout << x[i] << endl;
                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_title.c_str(), &x[i]);
                mpfr_set_d(mpfr_vector_x[i], x[i], MPFR_RNDN);

                ImGui::SameLine();

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_title.c_str(), &f[i]);
                mpfr_set_d(mpfr_vector_f[i], f[i], MPFR_RNDN);
            }
        }

        if (ImGui::Button("Calculate"))
        {   
            if (interval)
            {
                interval_naturalsplinevalue(n, interval_mp_x, interval_mp_f, interval_mp_xx, interval_mp_result, st);
            }

            if (float_point)
            {
                //cout << "test1" << endl;
                mpfr_set_d(mpfr_xx, xx, MPFR_RNDN);
                //cout << "test2" << endl;
                naturalsplinevalue(n, mpfr_vector_x, mpfr_vector_f, mpfr_xx, mpfr_result, st);
            }

            if (float_point_to_interval)
            {
                // zmiana floata na intervala
                float_to_interval(xx, x, f, interval_mp_x, interval_mp_f, interval_mp_xx);
                interval_naturalsplinevalue(n, interval_mp_x, interval_mp_f, interval_mp_xx, interval_mp_result, st);
            }
        }
    }
    
    if (factorGUI)
    {
        if (ImGui::Checkbox("Calculations in interval values", &interval)) {
            interval = true;
            float_point = false;
            float_point_to_interval = false;
        }

        if (ImGui::Checkbox("Calculations in float point values", &float_point)) {
            interval = false;
            float_point = true;
            float_point_to_interval = false;
        }

        if (ImGui::Checkbox("Calculations from float point to interval values", &float_point_to_interval)) {
            interval = false;
            float_point = false;
            float_point_to_interval = true;
        }

        if (float_point)
        {
            ImGui::SetNextItemWidth(100);
            if(ImGui::InputInt("Number of nodes:", &n)){
                if (n < 0)
                {
                    n = 0;
                }
                
                x.resize(n+1, 1.0); // resize with default value 1.0
                f.resize(n+1, 1.0); // resize with default value 1.0
            }

                        ImGui::Text("Point: %lf, Number of nodes: %d", xx, n);

            for (int i = 0; i < (int) x.size(); i++)
            {   
                std::string x_title = "x["+std::to_string(i)+"]";
                std::string f_title = "f["+std::to_string(i)+"]"; 
                // cout << x.size() << endl;
                // cout << i << endl;
                // cout << x[i] << endl;
                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_title.c_str(), &x[i]);
                mpfr_set_d(mpfr_vector_x[i], x[i], MPFR_RNDN);

                ImGui::SameLine();

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_title.c_str(), &f[i]);
                mpfr_set_d(mpfr_vector_f[i], f[i], MPFR_RNDN);
            }
        }

        if (interval)
        {
            ImGui::SetNextItemWidth(100);
            
            if(ImGui::InputInt("Number of nodes:", &n)){
                if (n < 0)
                {
                    n = 0;
                }
            
                interval_x.resize(n+1, {1.0, 1.0}); // resize with default value 1.0
                interval_f.resize(n+1, {1.0, 1.0}); // resize with default value 1.0
            }

            ImGui::Text("Number of nodes: %d", n);

            for (int i = 0; i < (int) interval_x.size(); i++)
            {   
                std::string x_left = "left x["+std::to_string(i)+"]";
                std::string x_title = "right x["+std::to_string(i)+"] ||";

                std::string f_left = "left f["+std::to_string(i)+"]";
                std::string f_title = "right f["+std::to_string(i)+"]"; 

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_left.c_str(), &interval_x[i].a);
                string ixa = to_string(interval_x[i].a);
                interval_mp_x[i].a = LeftRead<mpreal>(ixa);

                ImGui::SameLine();


                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_title.c_str(), &interval_x[i].b);
                string ixb = to_string(interval_x[i].b);
                interval_mp_x[i].b = LeftRead<mpreal>(ixb);

                ImGui::SameLine();

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_left.c_str(), &interval_f[i].a);
                string ifa = to_string(interval_f[i].a);
                interval_mp_f[i].a = LeftRead<mpreal>(ifa);

                ImGui::SameLine();

                
                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_title.c_str(), &interval_f[i].b);
                string ifb = to_string(interval_f[i].b);
                interval_mp_f[i].b = LeftRead<mpreal>(ifb);
            }
        }

        // float point to interval
        if (float_point_to_interval) {
            ImGui::SetNextItemWidth(100);

            if(ImGui::InputInt("Number of nodes:", &n)){
                if (n < 0)
                {
                    n = 0;
                }
                
                x.resize(n+1, 1.0); // resize with default value 1.0
                f.resize(n+1, 1.0); // resize with default value 1.0
            }

            ImGui::Text("Number of nodes: %d", n);

            for (int i = 0; i < (int) x.size(); i++)
            {   
                std::string x_title = "x["+std::to_string(i)+"]";
                std::string f_title = "f["+std::to_string(i)+"]"; 
                // cout << x.size() << endl;
                // cout << i << endl;
                // cout << x[i] << endl;
                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(x_title.c_str(), &x[i]);

                ImGui::SameLine();

                ImGui::SetNextItemWidth((int)(100));
                ImGui::InputDouble(f_title.c_str(), &f[i]);
            }
        }
        

        if (ImGui::Button("Calculate"))
        {   
            if (interval)
            {
                interval_naturalsplinecoeffns(n, interval_mp_x, interval_mp_f, a_mp, st);
            }

            if (float_point)
            {
               naturalsplinecoeffns(n, mpfr_vector_x, mpfr_vector_f, a_mpfr, st);
            }

            if (float_point_to_interval)
            {
                // zmiana floata na intervala
                float_to_interval(xx, x, f, interval_mp_x, interval_mp_f, interval_mp_xx);
                interval_naturalsplinecoeffns(n, interval_mp_x, interval_mp_f, a_mp,st);
            }
        }
    }
    
    

    ImGui::End();
    return 0;
}

int App::term(){
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
    return 0; 
}