#include "simulate.h"

void simulate::MouseClick()
{
    //POINT cursor_pos;
    //GetCursorPos(&cursor_pos);

    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    //input.mi.dx = (cursor_pos.x * 0xFFFF) / (GetSystemMetrics(SM_CXSCREEN) - 1);
    //input.mi.dy = (cursor_pos.y * 0xFFFF) / (GetSystemMetrics(SM_CYSCREEN) - 1);
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

    SendInput(1, &input, sizeof(INPUT));
}