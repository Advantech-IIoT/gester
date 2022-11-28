#ifndef INI_UTILITY_H
#define INI_UTILITY_H

#include <cstdarg>
#include <string>

#define BUFF_SIZE 1024
#define CMD_SIZE 1024

std::string get_2_finger_gesture_swipe_down_action();
std::string get_2_finger_gesture_swipe_up_action();
std::string get_2_finger_gesture_swipe_right_action();
std::string get_2_finger_gesture_swipe_left_action();
std::string get_3_finger_gesture_swipe_down_action();
std::string get_3_finger_gesture_swipe_up_action();
std::string get_3_finger_gesture_swipe_right_action();
std::string get_3_finger_gesture_swipe_left_action();
std::string get_4_finger_gesture_swipe_down_action();
std::string get_4_finger_gesture_swipe_up_action();
std::string get_4_finger_gesture_swipe_right_action();
std::string get_4_finger_gesture_swipe_left_action();
std::string get_5_finger_gesture_swipe_down_action();
std::string get_5_finger_gesture_swipe_up_action();
std::string get_5_finger_gesture_swipe_right_action();
std::string get_5_finger_gesture_swipe_left_action();

std::pair<std::string, int> execute_cmd(const char *cmd);
std::pair<std::string, bool> execute_cmd_get_single_info(const char *cmd, ...);
bool is_config_file_exist();

#endif // INI_UTILITY_H
