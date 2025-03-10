#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <math.h>
#include <fstream>

/*include my own config file in order to separate some variables from this file*/
#include "config.h"
#include "evtest_utility.h"
#include "ini_utility.h"

#define BUFSIZE 256

const char* GET_RESOLUTION_WIDTH_CMD = "cat /sys/class/drm/card*-*/modes | awk -F 'x' '{print $2}'";
const char* GET_RESOLUTION_HEIGHT_CMD = "cat /sys/class/drm/card*-*/modes | awk -F 'x' '{print $1}'";

sig_atomic_t sigusrsig = 0;
void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("Received SIGUSR1!\n");
        sigusrsig = 1;
    }
}

static int execute_command(const char *cmd, char *out_buffer, int out_len) {
	FILE *fp;
	memset(out_buffer, 0, out_len);

	if ((fp = popen(cmd, "r")) == NULL) {
		printf("Error opening pipe!\n");
		return -1;
	}
	fgets(out_buffer, out_len, fp);
	if (pclose(fp)) {
		printf("Command not found or exited with error status\n");
		return -1;
	}
	return 0;
}

int main(){
        // init touch device value
        init_touch_device_max(devname, xmax, ymax);
        bool isHideCursor = get_hide_cursor();
		int device = open(devname, O_RDONLY);
        struct input_event ev;
        /*holds information which finger is currently being tracked*/
        int finger = 0;
        /*holds information about the amount of fingers touching the screen*/
        int nfingers = 0;
        /*fix rotations with wide spread fingers*/
        int swipesuccess = 0;
        /*placeholder to help switching xmax and ymax*/
        int yrotmax = xmax;
        int xrotmax = ymax;
        /*get info from accelerometer script*/
        double accelscaling = 0.000009806;
        // std::string xaccelpathfull = accelpath + "iio:device0/" + xrawdata;
        // std::string yaccelpathfull = accelpath + "iio:device0/" + yrawdata;
        // std::ifstream xacceldata;
        // std::ifstream yacceldata;
        // xacceldata.open(xaccelpathfull);
        // yacceldata.open(yaccelpathfull);
        // if(xacceldata.good() != true){
        //     xaccelpathfull = accelpath + "iio:device1/" + xrawdata;
        //     yaccelpathfull = accelpath + "iio:device1/" + yrawdata;
        //     xacceldata.open(xaccelpathfull);
        //     yacceldata.open(yaccelpathfull);
        // }
        if (!is_config_file_exist()) {
            printf("missing gester config!\n");
            return -1;
        }

        std::string two_finger_gesture_swipe_down_action = get_2_finger_gesture_swipe_down_action();
        std::string two_finger_gesture_swipe_up_action = get_2_finger_gesture_swipe_up_action();
        std::string two_finger_gesture_swipe_right_action = get_2_finger_gesture_swipe_right_action();
        std::string two_finger_gesture_swipe_left_action = get_2_finger_gesture_swipe_left_action();
        std::string three_finger_gesture_swipe_down_action = get_3_finger_gesture_swipe_down_action();
        std::string three_finger_gesture_swipe_up_action = get_3_finger_gesture_swipe_up_action();
        std::string three_finger_gesture_swipe_right_action = get_3_finger_gesture_swipe_right_action();
        std::string three_finger_gesture_swipe_left_action = get_3_finger_gesture_swipe_left_action();
        std::string four_finger_gesture_swipe_down_action = get_4_finger_gesture_swipe_down_action();
        std::string four_finger_gesture_swipe_up_action = get_4_finger_gesture_swipe_up_action();
        std::string four_finger_gesture_swipe_right_action = get_4_finger_gesture_swipe_right_action();
        std::string four_finger_gesture_swipe_left_action = get_4_finger_gesture_swipe_left_action();
        std::string five_finger_gesture_swipe_down_action = get_5_finger_gesture_swipe_down_action();
        std::string five_finger_gesture_swipe_up_action = get_5_finger_gesture_swipe_up_action();
        std::string five_finger_gesture_swipe_right_action = get_5_finger_gesture_swipe_right_action();
        std::string five_finger_gesture_swipe_left_action = get_5_finger_gesture_swipe_left_action();
        // printf("%s\n%s\n%s\n%s\n", 
        //     two_finger_gesture_swipe_down_action.c_str(),
        //     two_finger_gesture_swipe_up_action.c_str(),
        //     two_finger_gesture_swipe_right_action.c_str(),
        //     two_finger_gesture_swipe_left_action.c_str());
        int two_finger_gesture_swipe_up_action_index = 8;
        int three_finger_gesture_swipe_up_action_index = 18;
        int four_finger_gesture_swipe_up_action_index = 28;
        int five_finger_gesture_swipe_up_action_index = 38;

        commands[two_finger_gesture_swipe_up_action_index] = two_finger_gesture_swipe_up_action.data();
        commands[two_finger_gesture_swipe_up_action_index+5] = two_finger_gesture_swipe_up_action.data();
        commands[two_finger_gesture_swipe_up_action_index+1] = two_finger_gesture_swipe_down_action.data();
        commands[two_finger_gesture_swipe_up_action_index+4] = two_finger_gesture_swipe_down_action.data();
        commands[two_finger_gesture_swipe_up_action_index+2] = two_finger_gesture_swipe_right_action.data();
        commands[two_finger_gesture_swipe_up_action_index+6] = two_finger_gesture_swipe_right_action.data();
        commands[two_finger_gesture_swipe_up_action_index+3] = two_finger_gesture_swipe_left_action.data();
        commands[two_finger_gesture_swipe_up_action_index+7] = two_finger_gesture_swipe_left_action.data();

        commands[three_finger_gesture_swipe_up_action_index] = three_finger_gesture_swipe_up_action.data();
        commands[three_finger_gesture_swipe_up_action_index+5] = three_finger_gesture_swipe_up_action.data();
        commands[three_finger_gesture_swipe_up_action_index+1] = three_finger_gesture_swipe_down_action.data();
        commands[three_finger_gesture_swipe_up_action_index+4] = three_finger_gesture_swipe_down_action.data();
        commands[three_finger_gesture_swipe_up_action_index+2] = three_finger_gesture_swipe_right_action.data();
        commands[three_finger_gesture_swipe_up_action_index+6] = three_finger_gesture_swipe_right_action.data();
        commands[three_finger_gesture_swipe_up_action_index+3] = three_finger_gesture_swipe_left_action.data();
        commands[three_finger_gesture_swipe_up_action_index+7] = three_finger_gesture_swipe_left_action.data();

        commands[four_finger_gesture_swipe_up_action_index] = four_finger_gesture_swipe_up_action.data();
        commands[four_finger_gesture_swipe_up_action_index+5] = four_finger_gesture_swipe_up_action.data();
        commands[four_finger_gesture_swipe_up_action_index+1] = four_finger_gesture_swipe_down_action.data();
        commands[four_finger_gesture_swipe_up_action_index+4] = four_finger_gesture_swipe_down_action.data();
        commands[four_finger_gesture_swipe_up_action_index+2] = four_finger_gesture_swipe_right_action.data();
        commands[four_finger_gesture_swipe_up_action_index+6] = four_finger_gesture_swipe_right_action.data();
        commands[four_finger_gesture_swipe_up_action_index+3] = four_finger_gesture_swipe_left_action.data();
        commands[four_finger_gesture_swipe_up_action_index+7] = four_finger_gesture_swipe_left_action.data();

        commands[five_finger_gesture_swipe_up_action_index] = five_finger_gesture_swipe_up_action.data();
        commands[five_finger_gesture_swipe_up_action_index+5] = five_finger_gesture_swipe_up_action.data();
        commands[five_finger_gesture_swipe_up_action_index+1] = five_finger_gesture_swipe_down_action.data();
        commands[five_finger_gesture_swipe_up_action_index+4] = five_finger_gesture_swipe_down_action.data();
        commands[five_finger_gesture_swipe_up_action_index+2] = five_finger_gesture_swipe_right_action.data();
        commands[five_finger_gesture_swipe_up_action_index+6] = five_finger_gesture_swipe_right_action.data();
        commands[five_finger_gesture_swipe_up_action_index+3] = five_finger_gesture_swipe_left_action.data();
        commands[five_finger_gesture_swipe_up_action_index+7] = five_finger_gesture_swipe_left_action.data();

        int accelxraw = 0 ;
        int accelyraw = 0 ;
        double accelx = 0 ;
        double accely = 0 ;
        double gthresh = 7.0 ;
        int orientation = 0;
        /*conversion of the angles from radians to degree*/
        double radtoang = 360/6.28318531;
        /*two arrays per finger to track*/
        std::vector<int> finger0x;
        std::vector<int> finger0y;
        std::vector<int> finger1x;
        std::vector<int> finger1y;
        std::vector<int> finger2x;
        std::vector<int> finger2y;
        std::vector<int> finger3x;
        std::vector<int> finger3y;
        std::vector<int> finger4x;
        std::vector<int> finger4y;
        // cursor
        std::vector<int> cursorX;
        std::vector<int> cursorY;

        // get screen resolution
        char buffer[BUFSIZE] = {0};
        char *pEnd;
        long int resolution_width = 0, resolution_height = 0;
        execute_command(GET_RESOLUTION_WIDTH_CMD, buffer, BUFSIZE);
        resolution_width = strtol(buffer, &pEnd, 10);
        execute_command(GET_RESOLUTION_HEIGHT_CMD, buffer, BUFSIZE);
        resolution_height = strtol(buffer, &pEnd, 10);
        std::string rotateScreen = get_rotate_screen();
        if (rotateScreen.compare(STRING_ROTATE_NINETY) == 0) {
            orientation = 1;
        } else if (rotateScreen.compare(STRING_ROTATE_ONE_EIGHTY) == 0) {
            orientation = 2;
        } else if (rotateScreen.compare(STRING_ROTATE_TWO_SEVENTY) == 0) {
            orientation = 3;
        } else {
            orientation = 0;
        }
        //printf("rotateScreen: %s orientation: %d\n", rotateScreen.c_str(), orientation);

        double x0last = 0, x1last = 0, x0first = 0, x0len = 0;
        double y0last = 0, y1last = 0, y0first = 0, y0len = 0;
        int translation_x = 0, last_translation_x = 0;
        int translation_y = 0, last_translation_y = 0;
        char move_cursor_command[BUFSIZE] = {0};

        signal(SIGUSR1, my_handler);
        while(1){
            if(sigusrsig == 1){
                sigusrsig = 0;
                // xacceldata.clear();
                // xacceldata.seekg(0);
                // yacceldata.clear();
                // yacceldata.seekg(0);
            }
            read(device,&ev, sizeof(ev));
                //if(ev.type == 1 && ev.value == 1){
                //        printf("Key: %i State: %i\n",ev.code,ev.value);
            //printf("=== type: %x code: %x  value: %2x\n", ev.type, ev.code, ev.value);
            /*count the number of fingers*/
            if(ev.code == ABS_MT_TRACKING_ID && ev.value >= 0){
                nfingers += 1;
            }
            /*determine which finger's coordinates will be incoming*/
            if(ev.code == ABS_MT_SLOT){
                finger = ev.value;
            }

            // cursor do not care orientation
            if (finger == 0){
                if(ev.code == ABS_MT_POSITION_X){
                    cursorX.push_back(ev.value);
                }
                if(ev.code == ABS_MT_POSITION_Y){
                    cursorY.push_back(ev.value);
                }
            }
            /*actually put the data into the arrays*/
            if(orientation == 0){
                //printf("normal orientation\n");
                if(finger == 0){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger0x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger0y.push_back(ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger1x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger1y.push_back(ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger2x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger2y.push_back(ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger3x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger3y.push_back(ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger4x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger4y.push_back(ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }else if(orientation == 1){
                //printf("rotated right\n");
                xmax = xrotmax;
                ymax = yrotmax;
                if(finger == 0){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger0x.push_back(xmax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger0y.push_back(ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger1x.push_back(ymax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger1y.push_back(ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger2x.push_back(ymax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger2y.push_back(ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger3x.push_back(ymax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger3y.push_back(ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger4x.push_back(ymax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger4y.push_back(ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }else if(orientation == 2){
                //printf("upside down\n");
                if(finger == 0){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger0x.push_back(xmax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger0y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger1x.push_back(xmax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger1y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger2x.push_back(xmax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger2y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger3x.push_back(xmax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger3y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == ABS_MT_POSITION_X){
                            finger4x.push_back(xmax - ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger4y.push_back(ymax - ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }else if (orientation == 3){
                //printf("rotated left\n");
                xmax = xrotmax;
                ymax = yrotmax;
                if(finger == 0){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger0x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger0y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger1x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger1y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger2x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger2y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger3x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger3y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == ABS_MT_POSITION_Y){
                            finger4x.push_back(ev.value);
                        }
                        if(ev.code == ABS_MT_POSITION_X){
                            finger4y.push_back(ymax - ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }
            //printf("array sizes %i,%i,%i,%i,%i\n",finger0x.size(),finger1x.size(),finger2x.size(),finger3x.size(),finger4x.size());

            // move cursor to touch position
            if (nfingers == 1 && cursorX.size() > 0 && cursorY.size() > 0 && !isHideCursor){
                x0last = cursorX[cursorX.size()-1];
                y0last = cursorY[cursorY.size()-1];
                translation_x = (int)(x0last / xmax * resolution_width);
                translation_y = (int)(y0last / ymax * resolution_height);
                // NOTE: using x || y changed would produce too many commands
                // it makes cursor movement very slow
                //if (translation_x != last_translation_x || translation_y != last_translation_y) {
                if (translation_x != last_translation_x) {
                    snprintf(move_cursor_command, sizeof(move_cursor_command) - 1, ydotool_abs_command, translation_x, translation_y);
                    system(move_cursor_command);
                    //printf("=== move_cursor_command=%s\n", move_cursor_command);
                }
                last_translation_x = translation_x;
                last_translation_y = translation_y;
            }

            if(ev.code == ABS_MT_TRACKING_ID && ev.value == -1){ /* this code+value means that a finger left the screen and I think that is when the gesture should end, not at the last finger.*/
                /*stuff to calculate per finger*/
                x0first = finger0x[0];
                x0last = finger0x[finger0x.size()-1];
                x0len = ( (x0last - x0first) / xmax );
                y0first = finger0y[0];
                y0last = finger0y[finger0y.size()-1];
                y0len = ( (y0last - y0first) / ymax );
                double finger0directionality = abs(x0len / y0len);
                //printf("directionality: %f\n",fing0directionality);
                double x1first,x1last,x1len,y1first,y1last,y1len,finger1directionality;
                double x2first,x2last,x2len,y2first,y2last,y2len,finger2directionality;
                double x3first,x3last,x3len,y3first,y3last,y3len,finger3directionality;
                double x4first,x4last,x4len,y4first,y4last,y4len,finger4directionality;


                /*only try reading from vectors that are not empty*/
                if(finger1x.size() != 0){
                    x1first = finger1x[0];
                    x1last = finger1x[finger1x.size()-1];
                    x1len = ( (x1last - x1first) / xmax );
                    y1first = finger1y[0];
                    y1last = finger1y[finger1y.size()-1];
                    y1len = ( (y1last - y1first) / ymax );
                    finger1directionality = abs(x1len / y1len);
                }
                if(finger2x.size() != 0){
                    x2first = finger2x[0];
                    x2last = finger2x[finger2x.size()-1];
                    x2len = ( (x2last - x2first) / xmax );
                    y2first = finger2y[0];
                    y2last = finger2y[finger2y.size()-1];
                    y2len = ( (y2last - y2first) / ymax );
                    finger2directionality = abs(x2len / y2len);
                }
                if(finger3x.size() != 0){
                    x3first = finger3x[0];
                    x3last = finger3x[finger3x.size()-1];
                    x3len = ( (x3last - x3first) / xmax );
                    y3first = finger3y[0];
                    y3last = finger3y[finger3y.size()-1];
                    y3len = ( (y3last - y3first) / ymax );
                    finger3directionality = abs(x3len / y3len);
                }
                if(finger4x.size() != 0){
                    x4first = finger4x[0];
                    x4last = finger4x[finger4x.size()-1];
                    x4len = ( (x4last - x4first) / xmax );
                    y4first = finger4y[0];
                    y4last = finger4y[finger4y.size()-1];
                    y4len = ( (y4last - y4first) / ymax );
                    finger4directionality = abs(x4len / y4len);
                }
                //printf("nfingers: %i\n",nfingers);

                    if(nfingers == 1){
                        /*one finger screen edge gestures*/
                        if(y0first >= (ymax - offsetbottom) && finger0directionality < 1){
                            printf("1 finger swipe from bottom edge!\n");
                            system(commands[0]);
                        }
                        if(y0first <= offsettop && finger0directionality < 1){
                            printf("1 finger swipe from top edge!\n");
                            system(commands[1]);
                        }
                        if(x0first >= (xmax - offsetright) && finger0directionality > 1){
                            printf("1 finger swipe from right edge!\n");
                            system(commands[2]);
                        }
                        if(x0first <= offsetleft && finger0directionality > 1){
                            printf("1 finger swipe from left edge!\n");
                            system(commands[3]);
                        }
                        if(y0last >= (ymax - offsetbottom) && finger0directionality < 1){
                            printf("1 finger swipe towards bottom edge!\n");
                            system(commands[4]);
                        }
                        if(y0last <= offsettop && finger0directionality < 1){
                            printf("1 finger swipe towards top edge!\n");
                            system(commands[5]);
                        }
                        if(x0last >= (xmax -offsetright) && finger0directionality > 1){
                            printf("1 finger swipe towards right edge!\n");
                            system(commands[6]);
                        }
                        if(x0last <= offsetleft && finger0directionality > 1){
                            printf("1 finger swipe towards left edge!\n");
                            system(commands[7]);
                        }
                    }
                    else if(nfingers == 2){
                            /*calculating values for 2 finger rotations*/
                            double xfirstcom = (x0first + x1first) / 2;
                            double yfirstcom = (y0first + y1first) / 2;
                            double xlastcom  = (x0last + x1last) / 2;
                            double ylastcom = (y0last + y1last) / 2;
                            double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                            printf("comdist: %f\n",comdist);
                            double vec0xfirst = x0first - xfirstcom;
                            double vec0yfirst = y0first - yfirstcom;
                            double vec1xfirst = x1first - xfirstcom;
                            double vec1yfirst = y1first - yfirstcom;
                            double vec0xlast = x0last - xlastcom;
                            double vec0ylast = y0last - ylastcom;
                            double vec1xlast = x1last - xlastcom;
                            double vec1ylast = y1last - ylastcom;
                            double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                            double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                            int angleavg = round((phi0 + phi1) / 2);
                            int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;
                            //printf("phi0: %f phi1: %f angleavg: %i anglescaled: %i\n",phi0,phi1,angleavg,anglescaled);
                            /*conditions for 2 finger gestures*/
                        if((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance)){
                            /*2 finger edge swipes*/
                            if(y0first >= (ymax - offsetbottom) && y1first >= (ymax - offsetbottom) && finger0directionality < 1 && finger1directionality <1){
                                printf("2 finger swipe from bottom edge!\n");
                                system(commands[8]);
                                swipesuccess = 1;
                            }else if(y0first <= offsettop && y1first <= offsettop && finger0directionality < 1 && finger1directionality < 1){
                                printf("2 finger swipe from top edge!\n");
                                system(commands[9]);
                                swipesuccess = 1;
                            }else if(x0first >= (xmax - offsetright) && x1first >= (xmax - offsetright) && finger0directionality > 1 && finger1directionality > 1){
                                printf("2 finger swipe from right edge!\n");
                                system(commands[10]);
                                swipesuccess = 1;
                            }else if(x0first <= offsetleft && x1first <= offsetleft && finger0directionality > 1 && finger1directionality > 1){
                                printf("2 finger swipe from left edge!\n");
                                system(commands[11]);
                                swipesuccess = 1;
                            }else if(finger0directionality < 1 && finger1directionality < 1 && y0last > y0first && y1last > y1first){
                                printf("2 finger swipe down!\n");
                                system(commands[12]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && y0last < y0first && y1last < y1first){
                                printf("2 finger swipe up!\n");
                                system(commands[13]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && x0last > x0first && x1last > x1first){
                                printf("2 finger swipe right!\n");
                                system(commands[14]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && x0last < x0first && x1last < x1first){
                                printf("2 finger swipe left!\n");
                                system(commands[15]);
                                swipesuccess = 1;
                            }

                        }
                        if(comdist < comdisttolerance && swipesuccess == 0){
                            printf("Two finger rotation with an angle of %i degree scaled to %i\n",angleavg,anglescaled);
                            if(angleavg < 0){
                                std::string tmpcommand = commands[16] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }else{
                                std::string tmpcommand = commands[17] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }
                        }
                    }
                    else if(nfingers == 3){
                        /*calculating values for 3 finger rotations*/
                        double xfirstcom = (x0first + x1first + x2first) / 3;
                        double yfirstcom = (y0first + y1first + y2first) / 3;
                        double xlastcom  = (x0last + x1last + x2last) / 3;
                        double ylastcom = (y0last + y1last + y2last) / 3;
                        double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                        //printf("comdist: %f\n",comdist);
                        double vec0xfirst = x0first - xfirstcom;
                        double vec0yfirst = y0first - yfirstcom;
                        double vec1xfirst = x1first - xfirstcom;
                        double vec1yfirst = y1first - yfirstcom;
                        double vec2xfirst = x2first - xfirstcom;
                        double vec2yfirst = y2first - yfirstcom;
                        double vec0xlast = x0last - xlastcom;
                        double vec0ylast = y0last - ylastcom;
                        double vec1xlast = x1last - xlastcom;
                        double vec1ylast = y1last - ylastcom;
                        double vec2xlast = x2last - xlastcom;
                        double vec2ylast = y2last - ylastcom;
                        double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                        double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                        double phi2 = atan2((vec2xfirst*vec2ylast - vec2yfirst*vec2xlast),(vec2xfirst*vec2xlast + vec2yfirst*vec2ylast))*radtoang;
                        int angleavg = round((phi0 + phi1 + phi2) / 3);
                        int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;
                        //printf("phi0: %f phi1: %f phi2: %f angleavg: %i anglescaled: %i\n",phi0,phi1,phi2,angleavg,anglescaled);

                        if(((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance && abs(x2len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance && abs(y2len) > swipetolerance))){
                            if(y0first >= (ymax - offsetbottom) && y1first >= (ymax - offsetbottom) && y2first >= (ymax-offsetbottom) && finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1){
                                printf("3 finger swipe from bottom edge!\n");
                                system(commands[18]);
                                swipesuccess = 1;
                            }else if(y0first <= offsettop && y1first <= offsettop && y2first <= offsettop && finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1){
                                printf("3 finger swipe from top edge!\n");
                                system(commands[19]);
                                swipesuccess = 1;
                            }else if(x0first >= (xmax - offsetright) && x1first >= (xmax - offsetright) && x2first >= (xmax - offsetright) && finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1){
                                printf("3 finger swipe from right edge!\n");
                                system(commands[20]);
                                swipesuccess = 1;
                            }else if(x0first <= offsetleft && x1first <= offsetleft && x2first <= offsetleft && finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1){
                                printf("3 finger swipe from left edge!\n");
                                system(commands[21]);
                                swipesuccess = 1;
                            }else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && y0last > y0first && y1last > y1first && y2last > y2first){
                                printf("3 finger swipe down!\n");
                                system(commands[22]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && y0last < y0first && y1last < y1first && y2last < y2first){
                                printf("3 finger swipe up!\n");
                                system(commands[23]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && x0last > x0first && x1last > x1first && x2last > x2first){
                                printf("3 finger swipe right!\n");
                                system(commands[24]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && x0last < x0first && x1last < x1first && x2last < x2first){
                                printf("3 finger swipe left!\n");
                                system(commands[25]);
                                swipesuccess = 1;
                            }
                        }
                        if(comdist < comdisttolerance && swipesuccess == 0){
                            printf("Three finger rotation with an angle of %i degree scaled to %i\n",angleavg,anglescaled);
                            if(angleavg < 0){
                                    std::string tmpcommand = commands[26] + std::to_string(anglescaled);
                                    system(tmpcommand.c_str());
                                }else{
                                    std::string tmpcommand = commands[27] + std::to_string(anglescaled);
                                    system(tmpcommand.c_str());
                                }
                        }
                    }
                    else if(nfingers == 4){
                        /*calculating values for 3 finger rotations*/
                        double xfirstcom = (x0first + x1first + x2first + x3first) / 4;
                        double yfirstcom = (y0first + y1first + y2first + y3first) / 4;
                        double xlastcom  = (x0last + x1last + x2last + x3last) / 4;
                        double ylastcom = (y0last + y1last + y2last +y3last) / 4;
                        double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                        //printf("comdist: %f\n",comdist);
                        double vec0xfirst = x0first - xfirstcom;
                        double vec0yfirst = y0first - yfirstcom;
                        double vec1xfirst = x1first - xfirstcom;
                        double vec1yfirst = y1first - yfirstcom;
                        double vec2xfirst = x2first - xfirstcom;
                        double vec2yfirst = y2first - yfirstcom;
                        double vec3xfirst = x3first - xfirstcom;
                        double vec3yfirst = y3first - yfirstcom;
                        double vec0xlast = x0last - xlastcom;
                        double vec0ylast = y0last - ylastcom;
                        double vec1xlast = x1last - xlastcom;
                        double vec1ylast = y1last - ylastcom;
                        double vec2xlast = x2last - xlastcom;
                        double vec2ylast = y2last - ylastcom;
                        double vec3xlast = x3last - xlastcom;
                        double vec3ylast = y3last - ylastcom;
                        double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                        double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                        double phi2 = atan2((vec2xfirst*vec2ylast - vec2yfirst*vec2xlast),(vec2xfirst*vec2xlast + vec2yfirst*vec2ylast))*radtoang;
                        double phi3 = atan2((vec3xfirst*vec3ylast - vec3yfirst*vec3xlast),(vec3xfirst*vec3xlast + vec3yfirst*vec3ylast))*radtoang;
                        int angleavg = round((phi0 + phi1 + phi2 + phi3) / 4);
                        int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;

                        if((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance && abs(x2len) > swipetolerance && abs(x3len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance && abs(y2len) > swipetolerance && abs(y3len) > swipetolerance)){
                            if(y0first >= (ymax - offsetbottom) && y1first >= (ymax - offsetbottom) && y2first >= (ymax - offsetbottom) && y3first >= (ymax - offsetbottom) && finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1){
                                printf("4 finger swipe from bottom edge!\n");
                                system(commands[28]);
                                swipesuccess = 1;
                            }else if(y0first <= offsettop && y1first <= offsettop && y2first <= offsettop && y3first <= offsettop && finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1){
                                printf("4 finger swipe from top edge!\n");
                                system(commands[29]);
                                swipesuccess = 1;
                            }else if(x0first >= (xmax - offsetright) && x1first >= (xmax - offsetright) && x2first >= (xmax - offsetright) && x3first >= (xmax - offsetright) && finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1){
                                printf("4 finger swipe from right edge!\n");
                                system(commands[30]);
                                swipesuccess = 1;
                            }else if(x0first <= offsetleft && x1first <= offsetleft && x2first <= offsetleft && x3first <= offsetleft && finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1){
                                printf("4 finger swipe from left edge!\n");
                                system(commands[31]);
                                swipesuccess = 1;
                            }else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last > y0first && y1last > y1first && y2last > y2first && y3last > y3first){
                                printf("4 finger swipe down!\n");
                                system(commands[32]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last < y0first && y1last < y1first && y2last < y2first && y3last < y3first){
                                printf("4 finger swipe up!\n");
                                system(commands[33]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && x0last > x0first && x1last > x1first && x2last > x2first && x3last > x3first){
                                printf("4 finger swipe right!\n");
                                system(commands[34]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && x0last < x0first && x1last < x1first && x2last < x2first && x3last < x3first){
                                printf("4 finger swipe left!\n");
                                system(commands[35]);
                                swipesuccess = 1;
                            }
                        }
                        if(comdist < comdisttolerance && swipesuccess == 0){
                            printf("Four finger rotation with an angle of %i degree scaled to %i\n",angleavg,anglescaled);
                            if(angleavg < 0){
                                std::string tmpcommand = commands[36] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }else{
                                std::string tmpcommand = commands[37] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }
                        }
                    }
                    else if(nfingers == 5){
                        /*calculating values for 3 finger rotations*/
                        double xfirstcom = (x0first + x1first + x2first + x3first + x4first) / 5;
                        double yfirstcom = (y0first + y1first + y2first + y3first + y4first) / 5;
                        double xlastcom  = (x0last + x1last + x2last + x3last + x4last) / 5;
                        double ylastcom  = (y0last + y1last + y2last + y3last + y4last) / 5;
                        double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                        //printf("comdist: %f\n",comdist);
                        double vec0xfirst = x0first - xfirstcom;
                        double vec0yfirst = y0first - yfirstcom;
                        double vec1xfirst = x1first - xfirstcom;
                        double vec1yfirst = y1first - yfirstcom;
                        double vec2xfirst = x2first - xfirstcom;
                        double vec2yfirst = y2first - yfirstcom;
                        double vec3xfirst = x3first - xfirstcom;
                        double vec3yfirst = y3first - yfirstcom;
                        double vec4xfirst = x4first - xfirstcom;
                        double vec4yfirst = y4first - yfirstcom;
                        double vec0xlast = x0last - xlastcom;
                        double vec0ylast = y0last - ylastcom;
                        double vec1xlast = x1last - xlastcom;
                        double vec1ylast = y1last - ylastcom;
                        double vec2xlast = x2last - xlastcom;
                        double vec2ylast = y2last - ylastcom;
                        double vec3xlast = x3last - xlastcom;
                        double vec3ylast = y3last - ylastcom;
                        double vec4xlast = x4last - xlastcom;
                        double vec4ylast = y4last - ylastcom;
                        double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                        double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                        double phi2 = atan2((vec2xfirst*vec2ylast - vec2yfirst*vec2xlast),(vec2xfirst*vec2xlast + vec2yfirst*vec2ylast))*radtoang;
                        double phi3 = atan2((vec3xfirst*vec3ylast - vec3yfirst*vec3xlast),(vec3xfirst*vec3xlast + vec3yfirst*vec3ylast))*radtoang;
                        double phi4 = atan2((vec4xfirst*vec4ylast - vec4yfirst*vec4xlast),(vec4xfirst*vec4xlast + vec4yfirst*vec4ylast))*radtoang;
                        int angleavg = round((phi0 + phi1 + phi2 + phi3 + phi4) / 5);
                        int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;

                        if((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance && abs(x2len) > swipetolerance && abs(x3len) > swipetolerance && abs(x4len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance && abs(y2len) > swipetolerance && abs(y3len) > swipetolerance && abs(y4len) > swipetolerance)){
                            if(y0first >= (ymax - offsetbottom) && y1first >= (ymax - offsetbottom) && y2first >= (ymax - offsetbottom) && y3first >= (ymax - offsetbottom) && finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1){
                                printf("5 finger swipe from bottom edge!\n");
                                system(commands[38]);
                                swipesuccess = 1;
                            }else if(y0first <= offsettop && y1first <= offsettop && y2first <= offsettop && y3first <= offsettop && finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1){
                                printf("5 finger swipe from top edge!\n");
                                system(commands[39]);
                                swipesuccess = 1;
                            }else if(x0first >= (xmax - offsetright) && x1first >= (xmax - offsetright) && x2first >= (xmax - offsetright) && x3first >= (xmax - offsetright) && finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1){
                                printf("5 finger swipe from right edge!\n");
                                system(commands[40]);
                                swipesuccess = 1;
                            }else if(x0first <= offsetleft && x1first <= offsetleft && x2first <= offsetleft && x3first <= offsetleft && finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1){
                                printf("5 finger swipe from left edge!\n");
                                system(commands[41]);
                                swipesuccess = 1;
                            }else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last > y0first && y1last > y1first && y2last > y2first && y3last > y3first){
                                printf("5 finger swipe down!\n");
                                system(commands[42]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last < y0first && y1last < y1first && y2last < y2first && y3last < y3first){
                                printf("5 finger swipe up!\n");
                                system(commands[43]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && x0last > x0first && x1last > x1first && x2last > x2first && x3last > x3first){
                                printf("5 finger swipe right!\n");
                                system(commands[44]);
                                swipesuccess = 1;
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && x0last < x0first && x1last < x1first && x2last < x2first && x3last < x3first){
                                printf("5 finger swipe left!\n");
                                system(commands[45]);
                                swipesuccess = 1;
                            }
                        }
                        if(comdist < comdisttolerance && swipesuccess == 0){
                            printf("Five finger rotation with an angle of %i degree scaled to %i\n",angleavg,anglescaled);
                            if(angleavg < 0){
                                std::string tmpcommand = commands[46] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }else{
                                std::string tmpcommand = commands[47] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }
                        }
                    }

                //printf("arrays before clear: %i %i %i %i %i\n",finger0x.size(),finger1x.size(),finger2x.size(),finger3x.size(),finger4x.size());

                /*empty the vectors*/
                finger0x.clear();
                finger0y.clear();
                finger1x.clear();
                finger1y.clear();
                finger2x.clear();
                finger2y.clear();
                finger3x.clear();
                finger3y.clear();
                finger4x.clear();
                finger4y.clear();
                cursorX.clear();
                cursorY.clear();

                x0first = 0;
                x0last = 0;
                x0len = 0;
                y0first = 0;
                y0last = 0;
                y0len = 0;
                finger0directionality = 0;
                x1first = 0;
                x1last = 0;
                x1len = 0;
                y1first = 0;
                y1last = 0;
                y1len = 0;
                finger1directionality = 0;
                x2first = 0;
                x2last = 0;
                x2len = 0;
                y2first = 0;
                y2last = 0;
                y2len = 0;
                finger2directionality = 0;
                x3first = 0;
                x3last = 0;
                x3len = 0;
                y3first = 0;
                y3last = 0;
                y3len = 0;
                finger3directionality = 0;
                x4first = 0;
                x4last = 0;
                x4len = 0;
                y4first = 0;
                y4last = 0;
                y4len = 0;
                finger4directionality = 0;

                //printf("arrays after clear: %i %i %i %i %i\n",finger0x.size(),finger1x.size(),finger2x.size(),finger3x.size(),finger4x.size());
                swipesuccess = 0;
                nfingers = 0;
            }
        }
}
