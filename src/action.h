#pragma once
#include <thread>

#include "imgui.h"
#include "utils.h"
#include "state.h"
#include "simulate.h"
#ifdef _DEBUG
#include <iostream>
#include <format>
#endif // DEBUG

namespace action
{
	void AutoClickWorker(std::stop_token s_token, utils::MouseButton btn_type, int interval_ms, int random_offset_ms);
	void AutoClickManager();
}