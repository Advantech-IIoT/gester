#ifndef INI_UTILITY_H
#define INI_UTILITY_H

#include <cstdarg>
#include <string>

#define STRING_BOOL_TRUE "true"
#define STRING_BOOL_FALSE "false"

#define STRING_ROTATE_NORMAL "normal"
#define STRING_ROTATE_NINETY "rotate-90"
#define STRING_ROTATE_ONE_EIGHTY "rotate-180"
#define STRING_ROTATE_TWO_SEVENTY "rotate-270"

#define BUFF_SIZE 1024
#define CMD_SIZE 1024

// gester related
bool is_config_file_exist();
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

// weston related
bool get_hide_cursor();
std::string get_rotate_screen();

// common functions
std::pair<std::string, int> execute_cmd(const char *cmd);
std::pair<std::string, bool> execute_cmd_get_single_info(const char *cmd, ...);
bool is_file_exist(const char *path);

#endif // INI_UTILITY_H
