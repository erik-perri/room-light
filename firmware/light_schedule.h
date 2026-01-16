#pragma once

#include "esphome.h"

struct LightState {
    int level;        // 0 to 4 (0 = Lowest, 4 = Highest)
    bool is_daylight; // false = Warm, true = Daylight
    bool power;       // true = ON, false = OFF
};

struct ScheduleEntry {
    int hour;         // 0-23
    int minute;       // 0-59
    LightState state;
};

// This must be sorted by time (00:00 -> 23:00)
const std::vector<ScheduleEntry> SCHEDULE = {
    // 05:30 -> Turn ON, Level 0 (Base), Warm
    {  5, 30, {0, false, true} },

    // 06:00 -> Level 1, Warm
    {  6, 00, {1, false, true} },

    // 07:00 -> Level 2, Warm
    {  7, 00, {2, false, true} },

    // 07:30 -> Level 3, Warm
    {  7, 30, {3, false, true} },

    // 08:30 -> Level 4 (Max), Daylight
    {  8, 30, {4, true,  true} },

    // 09:30 -> Turn OFF
    {  9, 30, {4, true,  false} },

    // 19:00 -> Level 4, Warm
    { 19, 00, {4, false, true} },

    // 20:00 -> Level 3, Warm
    { 20, 00, {3, false, true} },

    // 21:00 -> Level 2, Warm
    { 21, 00, {2, false, true} },

    // 22:00 -> Level 1, Warm
    { 22, 00, {1, false, true} },

    // 22:30 -> Level 0, Warm
    { 22, 30, {0, false, true} },

    // 23:00 -> Turn OFF
    { 23, 00, {0, false, false} }
};

LightState get_target_state(int current_hour, int current_minute) {
    int now_mins = current_hour * 60 + current_minute;
  
    // Default to the last entry in the list (handles the wrap-around from 23:00 to 00:00)
    LightState target = SCHEDULE.back().state;

    // Iterate through the schedule to find which block we are currently in
    for (const auto& entry : SCHEDULE) {
        int entry_mins = entry.hour * 60 + entry.minute;

        if (now_mins >= entry_mins) {
            target = entry.state;
        } else {
            // Since the list is sorted, if we have passed our current time, stop searching.
            break;
        }
    }

    return target;
}

bool is_schedule_breakpoint(int current_hour, int current_minute) {
    for (const auto& entry : SCHEDULE) {
        if (entry.hour == current_hour && entry.minute == current_minute) {
            return true;
        }
    }
    return false;
}
