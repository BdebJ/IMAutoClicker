#include "gui.h"
#include "action.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI gui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_LBUTTONDOWN:
        position = MAKEPOINTS(lParam); // set click points
        return 0;

    case WM_MOUSEMOVE: 
    {
        if (wParam == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(lParam);
            RECT rect = ::RECT{ };

            GetWindowRect(hwnd, &rect);

            rect.left += points.x - position.x;
            rect.top += points.y - position.y;

            if (position.x >= 0 &&
                position.x <= WIDTH &&
                position.y >= 0 && position.y <= 20)
                SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER );
        }
        return 0;
    }
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth  = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void gui::CreateMainWindow(const wchar_t* window_name, const wchar_t* class_name) noexcept
{
    wc.cbSize           = sizeof(wc);
    wc.style            = CS_CLASSDC;
    wc.lpfnWndProc      = WndProc;
    wc.cbClsExtra       = 0L;
    wc.cbWndExtra       = 0L;
    wc.hInstance        = GetModuleHandle(nullptr);
    wc.hIcon            = nullptr;
    wc.hCursor          = nullptr;
    wc.hbrBackground    = nullptr;
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = class_name;
    wc.hIconSm          = nullptr;

    ::RegisterClassExW(&wc);

    hwnd = ::CreateWindowW( class_name, 
                            window_name, 
                            WS_POPUP, 
                            100, 
                            100, 
                            WIDTH, 
                            HEIGHT, 
                            nullptr, 
                            nullptr, 
                            wc.hInstance, 
                            nullptr );
    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);
}

void gui::DestroyMainWindow() noexcept
{
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

bool gui::CreateDeviceD3D() noexcept
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount                          = 2;
    sd.BufferDesc.Width                     = 0;
    sd.BufferDesc.Height                    = 0;
    sd.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator     = 60;
    sd.BufferDesc.RefreshRate.Denominator   = 1;
    sd.Flags                                = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow                         = hwnd;
    sd.SampleDesc.Count                     = 1;
    sd.SampleDesc.Quality                   = 0;
    sd.Windowed                             = TRUE;
    sd.SwapEffect                           = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, 
                                                D3D_DRIVER_TYPE_HARDWARE, 
                                                nullptr, 
                                                createDeviceFlags, 
                                                featureLevelArray, 
                                                2, 
                                                D3D11_SDK_VERSION, 
                                                &sd, 
                                                &g_pSwapChain, 
                                                &g_pd3dDevice, 
                                                &featureLevel, 
                                                &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);

    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void gui::ResetDevice() noexcept
{
    ImGui_ImplDX11_InvalidateDeviceObjects();
    DestroyDeviceD3D();
    CreateDeviceD3D();
    ImGui_ImplDX11_CreateDeviceObjects();
}

void gui::DestroyDeviceD3D() noexcept
{
    CleanupRenderTarget();
    if (g_pSwapChain) 
    { 
        g_pSwapChain->Release(); 
        g_pSwapChain = nullptr; 
    }
    if (g_pd3dDeviceContext) 
    { 
        g_pd3dDeviceContext->Release(); 
        g_pd3dDeviceContext = nullptr; 
    }
    if (g_pd3dDevice) 
    { 
        g_pd3dDevice->Release(); 
        g_pd3dDevice = nullptr;
    }
}

void gui::CreateRenderTarget() noexcept
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void gui::CleanupRenderTarget() noexcept
{
    if (g_mainRenderTargetView) 
    { 
        g_mainRenderTargetView->Release(); 
        g_mainRenderTargetView = nullptr; 
    }
}

void gui::CreateImGui() noexcept
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io                  = &ImGui::GetIO();
    io->ConfigFlags     |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->IniFilename     = NULL;                                    // Disable config file generation

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}

void gui::DestroyImGui() noexcept
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT) { app_running = false; }
    }

    // Handle window resize (we don't resize directly in the WM_SIZE handler)
    if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
    {
        CleanupRenderTarget();
        g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
        g_ResizeWidth = g_ResizeHeight = 0;
        CreateRenderTarget();
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::Render() noexcept
{
    //State Vars
    static int click_interval[4] = { 0, 0, 0, 0 };
    static bool randomize_clicks_flag = false;
    static int random_time_ms = 0;

    const char* ms_btn_list[] = { "Left", "Middle", "Right" };
    static int ms_btn_selected = 0;

    const char* click_type_list[] = { "Single", "Double" };
    static int click_type_selected = 0;

    static int e = 0;

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ WIDTH, HEIGHT });

    ImGui::Begin("IM Auto Clicker", &app_running, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    ImGui::SeparatorText("Click Interval");
    {
        ImGui::PushItemWidth(-FLT_MIN);
        ImGui::InputInt4("", click_interval);
        ImGui::PopItemWidth();
        ImGui::Text("Hours         Mins          Secs          Milliseconds");

        ImGui::Dummy({0,10});
        ImGui::Checkbox("Randomize clicks", &randomize_clicks_flag);

        ImGui::SameLine();
        ImGui::Dummy({ 20,0 });

        ImGui::SameLine();
        ImGui::PushItemWidth(125);
        ImGui::InputInt("Offset in ms", &random_time_ms);
        ImGui::PopItemWidth();
    }
    ImGui::SeparatorText("Click Options");
    {
        ImGui::PushItemWidth(100);
        ImGui::Combo("Mouse Button", &ms_btn_selected, ms_btn_list, IM_ARRAYSIZE(ms_btn_list));
        ImGui::Combo("Click Type", &click_type_selected, click_type_list, IM_ARRAYSIZE(click_type_list));
        ImGui::PopItemWidth();
    }
    ImGui::SeparatorText("Cursor Position");
    {
        ImGui::RadioButton("Mouse Location", &e, 0); 
        ImGui::SameLine();
        ImGui::RadioButton("Pick Location", &e, 1);
    }

    if (ImGui::Button("Start")) 
    { 
        if (!autoclick_running)
        {
            autoclick_running = true;
            click_worker = std::jthread(action::AutoClickWorker, click_interval[3]);
            click_worker.detach();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        if (autoclick_running)
        {
            autoclick_running = false;
            click_worker.request_stop();
        }
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    ImGui::End();
}

void gui::EndRender() noexcept
{
    ImGui::Render();
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);

    // To add color to background
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0); // Present with vsync(1,0), without vsync(0,0)

    // Reset device in case of loss of D3D11 device
    HRESULT result = g_pd3dDevice->GetDeviceRemovedReason();
    if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
    {
        // If the device was removed or reset, attempt to recreate it
        ResetDevice();
    }
}