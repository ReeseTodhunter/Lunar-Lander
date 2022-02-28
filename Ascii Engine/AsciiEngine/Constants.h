#ifndef CONSTANTS_H
#define CONSTANTS_H

//Includes
#include<Windows.h>

//Defines
#define SCREEN_WIDTH 150
#define SCREEN_HEIGHT 40
#define FRAME_RATE 10

//Keys
// GAME CONSTANTS
const int KEY_ESC = VK_ESCAPE;
const int KEY_ENTER = VK_RETURN;
const int KEY_W = 'W';
const int KEY_A = 'A';
const int KEY_S = 'S';
const int KEY_D = 'D';
const int KEY_1 = '1';
const int KEY_2 = '2';
const int KEY_3 = '3';
const int KEY_4 = '4';

//PLayer Parameters
const float ACCELERATION_RATE = 0.5f;
const float DECELERATION_RATE = 0.25f;
const float FUEL_CONSUMPTION_RATE = 0.5f;
const int SCORE_INCREASE = 100;

#endif //CONSTANTS_H