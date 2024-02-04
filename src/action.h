#pragma once
#include <thread>

namespace action
{
	void AutoClickWorker(std::stop_token s_token, int interval);
}