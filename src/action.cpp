#include "action.h"
#include "simulate.h"

void action::AutoClickWorker(std::stop_token s_token, int interval)
{
    while (!s_token.stop_requested())
    {
        simulate::MouseClick();
        std::chrono::milliseconds sleep_time{ interval };
        std::this_thread::sleep_for(sleep_time);
    }
}