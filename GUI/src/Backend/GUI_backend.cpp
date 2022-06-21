#include "Backend/GUI_backend.h"

#include <fstream>
#include <string>
#include <stdio.h>

#include <filesystem>

#include "imgui.h"
//#include "Backend/imgui_impl_glfw.h"
//#include "Backend/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "Application.h"

#include "ConfigIni.h"

//Include the .cpp files containing the info for our fonts.
#include "../fonts/MonoLisa.cpp"
#include "../fonts/SevenSegment.cpp"

ImFont* _Font;
int FontSize = 15;

ImFont* _SevenSegmentFont;

bool _Closed = false;
bool _PreparingClose = false;

void CloseApplication()
{
    _Closed = true;
}

void CancelCloseApplication()
{
    _Closed = false;
    _PreparingClose = false;
}

ImFont* LoadFont(int size)
{
    ImGuiIO io = ImGui::GetIO();
    _Font = io.Fonts->AddFontFromMemoryCompressedTTF(MonoLisa_compressed_data, MonoLisa_compressed_size, size);


    return _Font;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void SaveDefaultImGuiIni()
{
    std::string defaultFileContent = R"(
[Window][DockSpaceViewport_11111111]
Pos=0,21
Size=1920,953
Collapsed=0

[Window][Debug##Default]
Pos=577,201
Size=481,363
Collapsed=0

[Window][Controls]
Pos=882,21
Size=311,179
Collapsed=0
DockId=0x00000009,0

[Window][Registers]
Pos=882,218
Size=311,133
Collapsed=0
DockId=0x0000000F,0

[Window][Code Editor]
Pos=0,21
Size=880,953
Collapsed=0
DockId=0x00000003,0

[Window][Hex]
Pos=1195,21
Size=725,953
Collapsed=0
DockId=0x00000002,0

[Window][Dear ImGui Demo]
Pos=650,20
Size=550,680
Collapsed=0

[Window][Example: Custom rendering]
Pos=615,115
Size=485,438
Collapsed=0

[Window][LEDs]
Pos=882,353
Size=311,114
Collapsed=0
DockId=0x0000000C,0

[Window][Switches]
Pos=882,469
Size=311,113
Collapsed=0
DockId=0x0000000D,0

[Window][7 Segment Display]
Pos=882,584
Size=311,123
Collapsed=0
DockId=0x00000005,0

[Window][Keyboard]
Pos=882,709
Size=311,265
Collapsed=0
DockId=0x0000000B,0

[Table][0xA1CC17D3,4]
Column 0  Weight=1.0000
Column 1  Weight=1.0000
Column 2  Weight=1.0000
Column 3  Weight=1.0000

[Table][0xD6F03CB8,2]
Column 0  Weight=1.0000
Column 1  Weight=1.0000

[Docking][Data]
DockSpace           ID=0x8B93E3BD Window=0xA787BDB4 Pos=0,55 Size=1920,953 Split=X
  DockNode          ID=0x00000001 Parent=0x8B93E3BD SizeRef=1193,720 Split=X Selected=0x529688BB
    DockNode        ID=0x00000003 Parent=0x00000001 SizeRef=880,974 CentralNode=1 Selected=0x529688BB
    DockNode        ID=0x00000004 Parent=0x00000001 SizeRef=311,974 Split=Y Selected=0xEAEE9E08
      DockNode      ID=0x00000007 Parent=0x00000004 SizeRef=303,573 Split=Y Selected=0xEAEE9E08
        DockNode    ID=0x00000006 Parent=0x00000007 SizeRef=305,337 Split=Y Selected=0x67284010
          DockNode  ID=0x00000009 Parent=0x00000006 SizeRef=311,199 Selected=0x67284010
          DockNode  ID=0x0000000F Parent=0x00000006 SizeRef=311,136 Selected=0xEAEE9E08
        DockNode    ID=0x0000000A Parent=0x00000007 SizeRef=305,234 Split=Y Selected=0x8F1A5834
          DockNode  ID=0x0000000C Parent=0x0000000A SizeRef=305,116 Selected=0x8F1A5834
          DockNode  ID=0x0000000D Parent=0x0000000A SizeRef=305,116 Selected=0xCF60CDE8
      DockNode      ID=0x00000008 Parent=0x00000004 SizeRef=303,399 Split=Y Selected=0xDA025FA3
        DockNode    ID=0x00000005 Parent=0x00000008 SizeRef=311,126 Selected=0xDA025FA3
        DockNode    ID=0x0000000B Parent=0x00000008 SizeRef=311,271 Selected=0xC5BEB8DE
  DockNode          ID=0x00000002 Parent=0x8B93E3BD SizeRef=725,720 Selected=0xD39109C3
)";

    std::ofstream file;
    file.open(".8085emu/imgui.ini", std::ios_base::out);
    file << defaultFileContent;
    file.close();
}

void window_maximize_callback(GLFWwindow* window, int maximized)
{
    ConfigIni::SetInt("Window", "Maximized", maximized);
}

int InitImGui()
{
    if (!std::filesystem::exists(".8085emu"))
    {
        std::filesystem::create_directory(".8085emu");
    }

    if (!std::filesystem::exists(".8085emu/imgui.ini"))
    {
        SaveDefaultImGuiIni();
    }

    ConfigIni::Init();

    //Setup window
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "8085 Emulator", NULL, NULL);

    if (window == NULL)
        return 1;

    glfwSetWindowMaximizeCallback(window, window_maximize_callback);

    if (ConfigIni::GetInt("Window", "Maximized", 1))
    {
        glfwMaximizeWindow(window);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
     // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = ".8085emu/imgui.ini";
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
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

    Application::Init();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    _Font = LoadFont(15);
    _SevenSegmentFont = io.Fonts->AddFontFromMemoryCompressedTTF(SevenSegment_compressed_data, SevenSegment_compressed_size, 50);

    // Main loop
    while (!_Closed)
    {
        if (glfwWindowShouldClose(window) && !_PreparingClose)
        {
            Application::PreDestroy();
            glfwSetWindowShouldClose(window, GLFW_FALSE);
            _PreparingClose = true;
        }

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        //-------------
        ImGui::PushFont(_Font);

        Application::ImGuiRender();

        ImGui::PopFont();
        //-------------

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    Application::Destroy();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}