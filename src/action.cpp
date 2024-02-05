#include "action.h"

void action::AutoClickWorker(std::stop_token s_token,utils::MouseButton btn_type, int interval_ms, int random_offset_ms)
{
    while (!s_token.stop_requested())
    {
        simulate::MouseClick(btn_type);

        int random_interval_ms = utils::GetRandomNum(random_offset_ms);
#ifdef _DEBUG
        printf("%d\n", random_interval_ms);
#endif // _DEBUG
        std::chrono::milliseconds sleep_time{ interval_ms + random_interval_ms };
        std::this_thread::sleep_for(sleep_time);
    }
}