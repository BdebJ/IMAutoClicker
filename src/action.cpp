#include "action.h"

void action::AutoClickWorker(std::stop_token s_token, utils::MouseButton btn_type, int interval_ms, int random_offset_ms)
{
	while (!s_token.stop_requested())
	{
		simulate::MouseClick(btn_type);

		int random_interval_ms = utils::GetRandomNum(random_offset_ms);
#ifdef _DEBUG
		printf("Sleeping for \033[1;32m%d\n\033[0m", interval_ms + random_interval_ms);
#endif // _DEBUG
		std::chrono::milliseconds sleep_time{ interval_ms + random_interval_ms };
		std::this_thread::sleep_for(sleep_time);
	}
}

// Runs on loop until app_running is false
void action::AutoClickManager()
{
	if (GetAsyncKeyState(VK_F6) & 1)
	{
		run_autoclick = !run_autoclick;
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
			printf("\nStarting autoclicks. \nInterval:\033[1;31m %d ms\033[0m \nInterval random offset:\033[1;31m %d ms\033[0m\n\n", interval_duration, (randomize_clicks_flag == true ? random_interval_offset : 0) );
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
			printf("\nStopping autoclicks\n\n");
#endif // _DEBUG
			thread_running = false;
			click_worker.request_stop();

		}
	}
}