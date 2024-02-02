#pragma once
#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
    constexpr int WIDTH = 500;
    constexpr int HEIGHT = 300;

	inline bool to_exit = false;
    inline ImGuiIO* io = nullptr;

    inline HWND hwnd = nullptr;
    inline WNDCLASSEXW wc = {};

    inline POINTS position = { };

    inline ID3D11Device* g_pd3dDevice = nullptr;
    inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    inline IDXGISwapChain* g_pSwapChain = nullptr;
    inline UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
    inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    //State vars
    inline bool show_demo_window = true;
    inline bool show_another_window = false;
    inline ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Win32 message handler
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Handle window creation and destruction
    void CreateMainWindow(const wchar_t* window_name, const wchar_t* class_name) noexcept;
    void DestroyMainWindow() noexcept;

    // Handle device creation, reset, and destruction
    bool CreateDeviceD3D() noexcept;
    void ResetDevice() noexcept;
    void CleanupDeviceD3D() noexcept;

    void CreateRenderTarget() noexcept;
    void CleanupRenderTarget() noexcept;

    // Handle ImGui initialization and cleanup
    void CreateImGui() noexcept;
    void DestroyImGui() noexcept;

    // Rendering with ImGui
    void BeginRender() noexcept;
    void EndRender() noexcept;
    void Render() noexcept;
}