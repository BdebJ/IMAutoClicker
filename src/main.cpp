#include "state.h"
#include "action.h"
#include "gui.h"

#ifdef _DEBUG
int main()
#endif // DEBUG
#ifdef NDEBUG
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
#endif // NDEBUG
{
    // create gui
    gui::CreateMainWindow(L"IMAutoClicker", L"AutoClicker");
    gui::CreateDeviceD3D();
    gui::CreateImGui();

    while (app_running)
    {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        action::AutoClickManager();
    }
    // destroy gui
    gui::DestroyImGui();
    gui::DestroyDeviceD3D();
    gui::DestroyMainWindow();

    return EXIT_SUCCESS;
}