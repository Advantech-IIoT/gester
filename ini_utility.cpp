#include <array>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ini_utility.h"

// gester related
const char* GESTER_CONFIG_PATH = "/etc/gester/gester.conf";

const char* TWO_FINGER_GESTURE_SECTION =   "2_finger_gesture";
const char* THREE_FINGER_GESTURE_SECTION = "3_finger_gesture";
const char* FOUR_FINGER_GESTURE_SECTION =  "4_finger_gesture";
const char* FIVE_FINGER_GESTURE_SECTION =  "5_finger_gesture";
const char* KEY_GESTURE_SWIPE_DOWN =  "swipe_down";
const char* KEY_GESTURE_SWIPE_UP =    "swipe_up";
const char* KEY_GESTURE_SWIPE_RIGHT = "swipe_right";
const char* KEY_GESTURE_SWIPE_LEFT =  "swipe_left";
const char* GESTURE_ACTION_SECTION =  "gesture_action";
const char* GET_INI_VALUE_CMD =       "/usr/local/bin/qtini -f %s -a read -s %s -k %s";

// weston related
const char* WESTON_CONFIG_FILE = "/etc/xdg/weston/weston.ini";

const char *WESTON_SECTION_LIBINPUT = "libinput";
const char *KEY_HIDE_CURSOR = "hide-cursor";

using namespace std;

static string _get_gesture_action_cmd(const char *finger_gesture, const char *gesture_key) {
    const auto retAction = execute_cmd_get_single_info(GET_INI_VALUE_CMD, GESTER_CONFIG_PATH, finger_gesture, gesture_key);
    if (!retAction.second) {
        printf("Failed to get %s:%s\n", finger_gesture, gesture_key);
        return retAction.first;
    }
    if (retAction.first.empty()) {
        return retAction.first;
    }
    const auto ret = execute_cmd_get_single_info(GET_INI_VALUE_CMD, GESTER_CONFIG_PATH, GESTURE_ACTION_SECTION, retAction.first.c_str());
    if (!ret.second) {
        printf("Failed to get %s:%s\n", GESTURE_ACTION_SECTION, retAction.first.c_str());
    }
    return ret.first;
}

bool is_config_file_exist()
{
    return is_file_exist(GESTER_CONFIG_PATH);
}
string get_2_finger_gesture_swipe_down_action() {
    return _get_gesture_action_cmd(TWO_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_DOWN);
}
string get_2_finger_gesture_swipe_up_action() {
    return _get_gesture_action_cmd(TWO_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_UP);
}
string get_2_finger_gesture_swipe_right_action() {
    return _get_gesture_action_cmd(TWO_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_RIGHT);
}
string get_2_finger_gesture_swipe_left_action() {
    return _get_gesture_action_cmd(TWO_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_LEFT);
}
string get_3_finger_gesture_swipe_down_action() {
    return _get_gesture_action_cmd(THREE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_DOWN);
}
string get_3_finger_gesture_swipe_up_action() {
    return _get_gesture_action_cmd(THREE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_UP);
}
string get_3_finger_gesture_swipe_right_action() {
    return _get_gesture_action_cmd(THREE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_RIGHT);
}
string get_3_finger_gesture_swipe_left_action() {
    return _get_gesture_action_cmd(THREE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_LEFT);
}
string get_4_finger_gesture_swipe_down_action() {
    return _get_gesture_action_cmd(FOUR_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_DOWN);
}
string get_4_finger_gesture_swipe_up_action() {
    return _get_gesture_action_cmd(FOUR_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_UP);
}
string get_4_finger_gesture_swipe_right_action() {
    return _get_gesture_action_cmd(FOUR_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_RIGHT);
}
string get_4_finger_gesture_swipe_left_action() {
    return _get_gesture_action_cmd(FOUR_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_LEFT);
}
string get_5_finger_gesture_swipe_down_action() {
    return _get_gesture_action_cmd(FIVE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_DOWN);
}
string get_5_finger_gesture_swipe_up_action() {
    return _get_gesture_action_cmd(FIVE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_UP);
}
string get_5_finger_gesture_swipe_right_action() {
    return _get_gesture_action_cmd(FIVE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_RIGHT);
}
string get_5_finger_gesture_swipe_left_action() {
    return _get_gesture_action_cmd(FIVE_FINGER_GESTURE_SECTION, KEY_GESTURE_SWIPE_LEFT);
}

bool get_hide_cursor()
{
    bool hideCursor = false;
    if (!is_file_exist(WESTON_CONFIG_FILE))
        return hideCursor;
    const auto ret = execute_cmd_get_single_info(GET_INI_VALUE_CMD, WESTON_CONFIG_FILE, WESTON_SECTION_LIBINPUT, KEY_HIDE_CURSOR);
    hideCursor = (ret.first.compare(0, strlen(STRING_BOOL_TRUE), STRING_BOOL_TRUE) == 0);
    return hideCursor;
}

pair<string, int> execute_cmd(const char *cmd)
{
    array<char, 256> buffer;
    string result;
    auto pipe = popen(cmd, "r");
    if (!pipe)
    {
        return make_pair(result, EXIT_FAILURE);
    }
    while (!feof(pipe))
    {
        if (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
            result += buffer.data();
    }
    auto rc = pclose(pipe);
    return make_pair(result, rc);
}

pair<string, bool> execute_cmd_get_single_info(const char *cmd, ...)
{
    bool result = false;
    char cmd_buff[CMD_SIZE] = {0};
    // check input
    if (!cmd || strlen(cmd) == 0) {
        string value_buff;
        return make_pair(value_buff, result);
    }

    // format command with args
    va_list args;
    va_start(args, cmd);
    vsnprintf(cmd_buff, CMD_SIZE, cmd, args);
    va_end(args);

    const auto ret = execute_cmd(cmd_buff);
    result = (ret.second == EXIT_SUCCESS);
    //printf("cmd: %s ret: %s result: %d\n", cmd_buff, ret.first.c_str(), result);
    return make_pair(ret.first, result);
}

bool is_file_exist(const char *path)
{
    if (access(path, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}
