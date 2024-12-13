#include "Globals.h"
#include "ModuleEditor.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleModelViewer.h"
#include "GL/glew.h"
#include "Windows.h"
#include "psapi.h"

ModuleEditor::ModuleEditor()
{
    
}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{
	ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindowModule()->window, App->GetOpenGLModule()->GetContext());
    ImGui_ImplOpenGL3_Init("#version 460");

    framerate = std::vector<float>(maximumPlotData, 0.f);
    frametime = std::vector<float>(maximumPlotData, 0.f);

    SDL_GetWindowSize(App->GetWindowModule()->window, &width, &height);

	return true;
}

update_status ModuleEditor::PreUpdate()
{
   
    

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float deltaTime)
{
    framerate[currentPlotData] = (1.f/deltaTime);
    frametime[currentPlotData] = deltaTime * 1000.f;

    Draw();

    currentPlotData++;

    if (currentPlotData >= maximumPlotData) currentPlotData = 0;

    if (closeApplication) return UPDATE_STOP;

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplSDL2_Shutdown(); 
    ImGui::DestroyContext();

    framerate.clear();
    frametime.clear();

    return true;
}

void ModuleEditor::ConfigMenu(bool& configMenu)
{
    ImGui::Begin("Configuration", &configMenu);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Options");
    if (ImGui::CollapsingHeader("Application"))
    {
        char title[25];
        sprintf_s(title, 25, "Framerate %.1f", framerate[currentPlotData]);
        ImGui::PlotHistogram("##framerate", &framerate[0], (int)framerate.size(), currentPlotData, title, 0.f, 240.f, ImVec2(310, 100));

        sprintf_s(title, 25, "Milliseconds %0.1f", frametime[currentPlotData]);
        ImGui::PlotHistogram("##milliseconds", &frametime[0], (int)frametime.size(), currentPlotData, title, 0.f, 40.f, ImVec2(310, 100));

        ImGui::Separator();

        ImGui::Text("Hardware");

        ImGui::Separator();

        // Displaying cpu info
        char info[256];
        sprintf_s(title, 25, "CPUs: ");
        sprintf_s(info, 256, "%i (Cache: %i kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

        ImGui::Text(title); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), info);

        // Ram data 
        sprintf_s(title, 25, "System RAM: ");
        float systemRAM = SDL_GetSystemRAM() / 1000.f;
        sprintf_s(info, 256, "%.1f GB", systemRAM);

        ImGui::Text(title); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), info);

        ImGui::Separator();

        // Libary versions
        SDL_version sldVerstion;
        SDL_GetVersion(&sldVerstion);

        sprintf_s(title, 25, "SDL Version: ");
        sprintf_s(info, 256, "%u.%u.%u", sldVerstion.major, sldVerstion.minor, sldVerstion.patch);

        ImGui::Text(title); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), info);

        sprintf_s(title, 25, "OpenGL Version: ");
        ImGui::Text(title); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (char*)glGetString(GL_VERSION));

        sprintf_s(title, 25, "GLSL Version: ");
        ImGui::Text(title); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));


    }

    if (ImGui::CollapsingHeader("Window"))
    {
        // FOV Slider
        if (ImGui::SliderInt("FOV", &fov, 60, 110))
        {
            App->GetCameraModule()->SetFOV((float)fov);
        }

        ImGui::Separator();

        // Resize sliders
        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);

        ImGui::SliderInt("Screen Width", &width, 600, displayMode.w);
        ImGui::SliderInt("Screen Height", &height, 400, displayMode.h);
        
        if (!fullscreen && !fullDesktop && resizableWindow && ImGui::Button("Resize"))
        {
            App->GetWindowModule()->ResizeWindow(width, height);
        }

        ImGui::Separator();

        // Window options
        if (ImGui::Checkbox("Fullscreen", &fullscreen))
        {
            Uint32 flag = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
            fullDesktop = false;
            App->GetWindowModule()->SetWindowFullscreen(flag);
        }

        ImGui::SameLine();
        if (ImGui::Checkbox("Allow Resize", &resizableWindow))
        {
            SDL_bool resizeable = resizableWindow ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE;
            App->GetWindowModule()->SetWindowResizable(resizeable);
        }

        if (ImGui::Checkbox("Borderless", &borderless))
        {
            SDL_bool bordered = borderless ? SDL_bool::SDL_FALSE : SDL_bool::SDL_TRUE;
            fullscreen = false;
            fullDesktop = false;
            App->GetWindowModule()->SetWindowFullscreen(0);
            App->GetWindowModule()->SetWindowBorderless(bordered);
        }
        
        ImGui::SameLine();
        if (ImGui::Checkbox("Full desktop", &fullDesktop))
        {
            Uint32 flag = fullDesktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
            fullscreen = false;
            App->GetWindowModule()->SetWindowFullscreen(flag);
        }
    }

    if (ImGui::CollapsingHeader("OpenGL"))
    {
        if (ImGui::SliderFloat("Red Clear Color", &clearRed, 0.f, 1.f)) App->GetOpenGLModule()->SetClearRed(clearRed);
        if (ImGui::SliderFloat("Green Clear Color", &clearGreen, 0.f, 1.f)) App->GetOpenGLModule()->SetClearGreen(clearGreen);
        if (ImGui::SliderFloat("Blue Clear Color", &clearBlue, 0.f, 1.f)) App->GetOpenGLModule()->SetClearBlue(clearBlue);

        if (ImGui::Button("Reset Colors"))
        {
            clearRed = DEFAULT_GL_CLEAR_COLOR_RED;
            clearGreen = DEFAULT_GL_CLEAR_COLOR_GREEN;
            clearBlue = DEFAULT_GL_CLEAR_COLOR_BLUE;

            App->GetOpenGLModule()->SetClearRed(clearRed);
            App->GetOpenGLModule()->SetClearGreen(clearGreen);
            App->GetOpenGLModule()->SetClearBlue(clearBlue);
        }

        ImGui::Separator();

        if (ImGui::Checkbox("Enable back face culling", &backFaceCulling))
        {
            if (backFaceCulling) glEnable(GL_CULL_FACE);
            else glDisable(GL_CULL_FACE);
        }

        ImGui::SameLine();

        if (ImGui::Checkbox("CCW_Faces", &CCW_Face))
        {
            if (CCW_Face) glFrontFace(GL_CCW);
            else glFrontFace(GL_CW);
        }

        ImGui::SameLine();

        if (ImGui::Checkbox("Enable depth test", &depthTest))
        {
            if (depthTest) glEnable(GL_DEPTH_TEST);
            else glDisable(GL_DEPTH_TEST);
        }
    }

    if (ImGui::CollapsingHeader("Textures"))
    {
        int totalLoadedTextures = App->GetModelViewerModule()->GetModelLoadedTextures();
        int currentLoadedTexture = App->GetModelViewerModule()->GetModelRenderTexture();
        currentLoadedTexture = currentLoadedTexture < 0 ? 0 : currentLoadedTexture;
        bool disableSlider = totalLoadedTextures < 2;

        if (disableSlider) ImGui::BeginDisabled();

        if (ImGui::SliderInt("Select texture to render", &currentLoadedTexture, 0, totalLoadedTextures - 1)) App->GetModelViewerModule()->SetModelRenderTexture(currentLoadedTexture);

        if (disableSlider) ImGui::EndDisabled();
    }

    if (ImGui::CollapsingHeader("Camera"))
    {
        ModuleCamera* cameraModule = App->GetCameraModule();

        float movementScaleFactor = cameraModule->GetMoveMentScaleFactor();
        float cameraMoveSpeed = cameraModule->GetCameraMoveSpeed();
        float mouseSens = cameraModule->GetMouseSensitivity();
        float zoomSens = cameraModule->GetZoomSensitivity();

        // Movement Sacle Factor
        if (ImGui::SliderFloat("Movement Scale Factor", &movementScaleFactor, 0.1f, 5.f)) cameraModule->SetMoveMentScaleFactor(movementScaleFactor);
        if (ImGui::Button("Reset Scale"))
        {
            movementScaleFactor = DEFAULT_CAMERA_MOVEMENT_SCALE_FACTOR;
            cameraModule->SetMoveMentScaleFactor(movementScaleFactor);
        }

        ImGui::Separator();

        // Camera Move Speed
        if (ImGui::SliderFloat("Camera Move Speed", &cameraMoveSpeed, 0.1f, 100.f)) cameraModule->SetCameraMoveSpeed(cameraMoveSpeed);
        if (ImGui::Button("Reset Move Speed"))
        {
            cameraMoveSpeed = DEFAULT_CAMERA_MOVMENT_SPEED;
            cameraModule->SetCameraMoveSpeed(cameraMoveSpeed);
        }

        ImGui::Separator();

        // Mouse sensitivity
        if (ImGui::SliderFloat("Mouse Sensitivity", &mouseSens, 0.1f, 1.f)) cameraModule->SetMouseSensitivity(mouseSens);
        if (ImGui::Button("Reset Mouse Sens"))
        {
            mouseSens = DEFAULT_CAMERA_MOUSE_SENSITIVITY;
            cameraModule->SetMouseSensitivity(mouseSens);
        }

        ImGui::Separator();

        // Zoom Sensitivity
        if (ImGui::SliderFloat("Zoom Sensitivity", &zoomSens, 0.1f, 100.f)) cameraModule->SetZoomSensitivity(zoomSens);
        if (ImGui::Button("Reset Zoom Sens"))
        {
            zoomSens = DEFAUL_CAMERA_ZOOM_SENSITIVITY;
            cameraModule->SetZoomSensitivity(zoomSens);
        }

        if (ImGui::Button("Reset Values"))
        {
            movementScaleFactor = DEFAULT_CAMERA_MOVEMENT_SCALE_FACTOR;
            cameraMoveSpeed = DEFAULT_CAMERA_MOVMENT_SPEED;
            mouseSens = DEFAULT_CAMERA_MOUSE_SENSITIVITY;
            zoomSens = DEFAUL_CAMERA_ZOOM_SENSITIVITY;

            cameraModule->SetMoveMentScaleFactor(movementScaleFactor);
            cameraModule->SetCameraMoveSpeed(cameraMoveSpeed);
            cameraModule->SetMouseSensitivity(mouseSens);
            cameraModule->SetZoomSensitivity(zoomSens);
        }
    }

    ImGui::Separator();

    if (ImGui::Button("Close Me")) configMenu = false;
    ImGui::TextLinkOpenURL("https://github.com/");

    ImGui::End();
}

void ModuleEditor::Console(bool& console)
{
    ImGui::Begin("Console", &console);

    for (const char* log : *Logs)
    {
        ImGui::TextUnformatted(log);
    }

    ImGui::End();
}

void ModuleEditor::SetNewScreenSize(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
}

void ModuleEditor::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Menu"))
    {
        if (ImGui::MenuItem("Gui Demo"))
            showcaseMenu = !showcaseMenu;

        if (ImGui::MenuItem("Configuration"))
            configMenu = !configMenu;

        if (ImGui::MenuItem("Console"))
            consoleMenu = !consoleMenu;

        if (ImGui::MenuItem("Close Application")) closeApplication = true;

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (showcaseMenu) ImGui::ShowDemoWindow(&showcaseMenu);
    if (configMenu) ConfigMenu(configMenu);
    if (consoleMenu) Console(consoleMenu);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
