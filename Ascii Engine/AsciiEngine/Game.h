#ifndef GAME_H
#define GAME_H

//Includes
#include <Windows.h>
#include "GameObjects.h"

class Game 
{
public:
	//Functions
	Game();
	void Intitialise();
	void Update(float deltaTime);
	void Draw();

	//Variables
	bool exitGame;

private:
	//ENUMS
	enum GAME_STATE
	{
		SPLASH,
		MAIN_MENU,
		OPTIONS,
		SCOREBOARD,
		GAME
	};

	//Console Variables 
	SMALL_RECT windowSize{ 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	COORD bufferSize = { SCREEN_WIDTH , SCREEN_HEIGHT };
	COORD characterBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD characterPosition = { 0, 0 };
	SMALL_RECT consoleWriteArea = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	CHAR_INFO consoleBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);

	// Game Variables
	GAME_STATE currentGameState = SPLASH;
	Background background;
	Player player;
	Explosion explosion;
	Thrust thrust;
	bool gamePaused;
};

#endif //GAME_H
