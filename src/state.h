#pragma once
#include <thread>

// Window size
constexpr int WIDTH = 400;
constexpr int HEIGHT = 300;

// App state vars
inline std::jthread click_worker;       // Thread to delegate autoclick

inline bool app_running = true;         // App exit flag
inline bool run_autoclick = false;		// Autoclick start/stop flag
inline bool thread_running = false;		// (DO NOT CHANGE - Managed by AutoClickManager fn) Autoclick worker thread start/stop flag 

// GUI State Vars
inline int click_interval[4] = { 0, 0, 0, 50 };

inline bool randomize_clicks_flag = false;
inline int random_interval_offset = 0;

inline const char* ms_btn_list[] = { "Left", "Middle", "Right" };
inline int ms_btn_selected = 0;

inline int click_loc_type = 0;

//inline const char* click_type_list[] = { "Single", "Double" };
//inline int click_type_selected = 0;