#include "action.h"

// Runs on loop until app_running is false
void action::AutoClickManager()
{
	if (GetAsyncKeyState(VK_F6) & 1)
	{
		run_autoclick = !run_autoclick;
	}

	if (click_loc_type)
	{
		if (!set_loc_flag)
		{
			POINT cursor_pos;
			GetCursorPos(&cursor_pos);
			ms_loc[0] = cursor_pos.x;
			ms_loc[1] = cursor_pos.y;
			if (GetAsyncKeyState(VK_LBUTTON) & KF_UP)
			{
				set_loc_flag = true;
			}
		}
	}
	else
	{
		set_loc_flag = false;
	}

	if (run_autoclick)
	{
		if (!thread_running)
		{
			thread_running = true;

			int interval_duration = (click_interval[0] * 60 * 60 * 1000) +
				(click_interval[1] * 60 * 1000) +
				(click_interval[2] * 1000) +
				click_interval[3];
#ifdef _DEBUG
			std::cout << std::format("\nStarting autoclicks. \nInterval: {} ms \nInterval random offset: {} ms\n\n", interval_duration, (randomize_clicks_flag == true ? random_interval_offset : 0));
#endif // _DEBUG
			if (randomize_clicks_flag)
			{
				click_worker = std::jthread(action::AutoClickWorker, static_cast<utils::MouseButton>(ms_btn_selected), interval_duration, random_interval_offset);
			}
			else
			{
				click_worker = std::jthread(action::AutoClickWorker, static_cast<utils::MouseButton>(ms_btn_selected), interval_duration, 0);
			}
			click_worker.detach();
		}
	}
	else
	{
		if (thread_running)
		{
#ifdef _DEBUG
			std::cout << "\nStopping autoclicks\n";
#endif // _DEBUG
			click_worker.request_stop();
			thread_running = false;
		}
	}
}

void action::AutoClickWorker(std::stop_token s_token, utils::MouseButton btn_type, int interval_ms, int random_offset_ms)
{
	int debug_click_count = 0;
	while (!s_token.stop_requested() && (click_repeat_type ? click_repeat_count : true))
	{
		simulate::MouseClick(btn_type);
		debug_click_count++;
		if (click_repeat_type == 1 && click_repeat_count > 0)
		{
			click_repeat_count--;
		}

		int random_interval_ms = utils::GetRandomNum(random_offset_ms);
		int actual_interval = interval_ms + random_interval_ms < MIN_INTERVAL ? MIN_INTERVAL : interval_ms + random_interval_ms;
#ifdef _DEBUG
		std::cout << std::format("Sleeping for {} ms\n", actual_interval);
		std::cout << std::format("Clicked {} times\n", debug_click_count);
#endif // _DEBUG
		std::chrono::milliseconds sleep_time{ actual_interval };
		std::this_thread::sleep_for(sleep_time);
	}
	return;
}
