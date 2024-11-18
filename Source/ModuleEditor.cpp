#include "ModuleEditor.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"

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
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->GetContext());
    ImGui_ImplOpenGL3_Init("#version 460");

	return true;
}

update_status ModuleEditor::PreUpdate()
{
   
    

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    Draw();

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplSDL2_Shutdown(); 
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::ConfigMenu(bool& configMenu)
{
    ImGui::Begin("Configuration", &configMenu);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Options");
    if (ImGui::CollapsingHeader("Application"))
    {
        
    }

    if (ImGui::CollapsingHeader("Window"))
    {
        ImGui::SliderInt("Values", &value, 0, 2);
        ImGui::Checkbox("Checkboxed", &checkBoxed);
    }

    if (ImGui::Button("Close Me")) configMenu = false;

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

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (showcase) ImGui::ShowDemoWindow(&showcase);
    if (configMenu) ConfigMenu(configMenu);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
