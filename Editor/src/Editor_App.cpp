// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>

#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <iostream>

struct Test {
    bool clicked = false;
    ImVec2 pos;
    int focus =0;

    ImGuiContext * img;

};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void Window_Start(GLFWwindow* ptr, ImGuiContext * ctx) {

    glfwMakeContextCurrent(ptr);

    ImGui::SetCurrentContext(ctx); // workaround
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void Window_End(GLFWwindow* ptr, ImGuiContext * ctx) {

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Rendering
    auto & io  = ImGui::GetIO();
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(ptr, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(ptr);

}

GLFWwindow * Create_Window(Test * user_data, const std::string & name, GLFWwindow * window2 = nullptr) {

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, name.c_str(), NULL, window2);
    if (window == NULL)
        return nullptr;
    glfwSetWindowUserPointer(window, user_data);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync


    //https://gamedev.stackexchange.com/questions/151560/modifying-glfw-callbacks

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
    {
        Test& data = *(Test*)glfwGetWindowUserPointer(window);


        auto backup = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(data.img);

        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        ImGui::SetCurrentContext(backup);
//        switch (action)
//        {
//            case GLFW_PRESS:
//            {
//                data.clicked = true;
//                break;
//            }
//            case GLFW_RELEASE:
//            {
//                data.clicked = false;
//                break;
//            }
//        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
    {
        Test& data = *(Test*)glfwGetWindowUserPointer(window);

        auto backup = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(data.img);

        ImGui_ImplGlfw_CursorPosCallback(window, x, y);
        ImGui::SetCurrentContext(backup);

        int window_x, window_y;
        glfwGetWindowPos(window, &window_x, &window_y);
        x += window_x;
        y += window_y;

        data.pos.x = (float)x;
        data.pos.y = (float)y;
    });

    glfwSetWindowFocusCallback(window, [](GLFWwindow * window, int focus) {
        Test& data = *(Test*)glfwGetWindowUserPointer(window);

        auto backup = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(data.img);
        //temporary...

        if (data.img && ImGui::GetIO().BackendPlatformUserData != NULL)
            ImGui_ImplGlfw_WindowFocusCallback(window, focus);
        ImGui::SetCurrentContext(backup);
        data.focus = focus;

    });

    glfwSetCursorEnterCallback(window, [](GLFWwindow * window, int entered) {
        Test& data = *(Test*)glfwGetWindowUserPointer(window);

        auto backup = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(data.img);
        //temporary...

//        if (data.img && ImGui::GetIO().BackendPlatformUserData != NULL)
            ImGui_ImplGlfw_CursorEnterCallback(window, entered);
        ImGui::SetCurrentContext(backup);

    });

    // Create window with graphics context
    return window;

}

ImGuiContext * Create_Context(bool enable_docking = true, bool enable_viewports = false) {

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::SetCurrentContext(nullptr); // workaround
    auto ctx1 = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    if (enable_viewports) // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    if (enable_docking)
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    ImGui::StyleColorsDark();

    return ctx1;
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    Test * mouse1 = new Test();
    Test * mouse2 = new Test();
    GLFWwindow * window1 = Create_Window(mouse1, "Dear ImGui GLFW+OpenGL3 example 1");
    ImGuiContext * ctx1 = Create_Context(true, true);
    mouse1->img = ctx1;
    GLFWwindow * window2 = Create_Window(mouse2, "Dear ImGui GLFW+OpenGL3 example 2", window1);
    ImGuiContext * ctx2 = Create_Context(true , false);
    mouse2->img = ctx2;

    //so basically this should work.. since we can get our data

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.

    glfwMakeContextCurrent(window1);
    ImGui::SetCurrentContext(ctx1); // workaround
    ImGui_ImplGlfw_InitForOpenGL(window1, false);
    ImGui_ImplOpenGL3_Init(glsl_version);




    glfwMakeContextCurrent(window2);
    ImGui::SetCurrentContext(ctx2); // workaround
    ImGui_ImplGlfw_InitForOpenGL(window2, false);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window1))
    {




        auto & io = ImGui::GetIO();

        //We start IMGUI for first window..
        Window_Start(window1, ctx1);
        glfwPollEvents(); //POLLS ALL WINDOWS DID NOT KNOW THAT..
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f1 = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f1, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        Window_End(window1, ctx1);


        Window_Start(window2, ctx2);

        if (show_demo_window)
           ImGui::ShowDemoWindow(&show_demo_window);

            ImGui::Button("test");
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f2 = 0.0f;
            static int counter = 0;

            ImGui::Begin("The Heck, Test!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f2, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        Window_End(window2, ctx2);



    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::SetCurrentContext(ctx1);
    ImGui::DestroyContext();
    ImGui::SetCurrentContext(ctx2);
    ImGui::DestroyContext();

    glfwDestroyWindow(window1);
    glfwDestroyWindow(window2);
    glfwTerminate();

    return 0;
}
