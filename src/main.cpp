#include "gui.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // create gui
    gui::CreateMainWindow(L"ImGui Example", L"Dear ImGui DirectX11 Example");
    gui::CreateDeviceD3D();
    gui::CreateImGui();

    while (gui::app_running)
    {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();
    }
    // destroy gui
    gui::DestroyImGui();
    gui::DestroyDeviceD3D();
    gui::DestroyMainWindow();

    return EXIT_SUCCESS;
}