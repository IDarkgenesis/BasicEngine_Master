#include "Globals.h"
#include "ModuleEditor.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"

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

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->GetContext());
    ImGui_ImplOpenGL3_Init("#version 460");

    framerate = std::vector<float>(maximumPlotData, 0.f);
    frametime = std::vector<float>(maximumPlotData, 0.f);

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
        ImGui::PlotHistogram("##framerate", &framerate[0], framerate.size(), currentPlotData, title, 0.f, 240.f, ImVec2(310, 100));

        sprintf_s(title, 25, "Milliseconds %0.1f", frametime[currentPlotData]);
        ImGui::PlotHistogram("##milliseconds", &frametime[0], frametime.size(), currentPlotData, title, 0.f, 40.f, ImVec2(310, 100));
    }

    if (ImGui::CollapsingHeader("Window"))
    {
        if (ImGui::SliderInt("FOV", &fov, 60, 110))
        {
            App->GetCamera()->SetFOV(fov);
        }
    }

    if (ImGui::Button("Close Me")) configMenu = false;

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

void ModuleEditor::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Help!"))
    {
        if (ImGui::MenuItem("Gui Demo"))
            showcase = !showcase;

        if (ImGui::MenuItem("Configuration"))
            configMenu = !configMenu;

        if (ImGui::MenuItem("Console"))
            console = !console;

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (showcase) ImGui::ShowDemoWindow(&showcase);
    if (configMenu) ConfigMenu(configMenu);
    if (console) Console(console);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
