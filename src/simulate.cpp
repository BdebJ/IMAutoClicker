#include "simulate.h"

void simulate::MouseClick(utils::MouseButton btn_type)
{
    //POINT cursor_pos;
    //GetCursorPos(&cursor_pos);
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    //input.mi.dx = (cursor_pos.x * 0xFFFF) / (GetSystemMetrics(SM_CXSCREEN) - 1);
    //input.mi.dy = (cursor_pos.y * 0xFFFF) / (GetSystemMetrics(SM_CYSCREEN) - 1);

    if (click_loc_type == 1)
    {
        SetCursorPos(ms_loc[0], ms_loc[1]);
    }

    switch (btn_type)
    {
    case(utils::MouseButton::LEFT):
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
        break;
    case(utils::MouseButton::MIDDLE):
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP;
        break;
    case(utils::MouseButton::RIGHT):
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
        break;
    }

    for (int i = 0; i < click_type_selected + 1; i++)
    {
        SendInput(1, &input, sizeof(INPUT));
    }
}