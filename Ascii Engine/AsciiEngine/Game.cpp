//This classes header
#include "Game.h"

//Includes
#include "Utility.h"
#include "Constants.h"

Game::Game()
{
	//Constructor
}

void Game::Intitialise()
{
	// Set the console title
	SetConsoleTitle(L"Lunar LANDER!");
	// Set the screen buffer Size
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	// Set the window size
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	//Set exitGame to false
	bool exitGame = false;
	//initialise gamePaused
	bool gamePaused = false;
}

void Game::Update(float deltaTime)
{
	switch (currentGameState)
	{
		case SPLASH:
		{
			//create new splash screen only inside SPLASH
			static Splash splash;

			if (splash.Duration < 5.0f)
			{
				//Calculate the splash duration
				splash.Duration += deltaTime;
				//Draw the splash image
				WriteImageToBuffer(consoleBuffer, splash.CHARACTERS, splash.COLOURS, splash.HEIGHT, splash.WIDTH,
					(SCREEN_WIDTH / 2) - (splash.WIDTH / 2), (SCREEN_HEIGHT / 2) - (splash.HEIGHT / 2));
			}
			else
			{
				//Move to the Main Menu
				splash.Duration = 0.0f;
				currentGameState = MAIN_MENU;
			}
			break;
		}
		case MAIN_MENU:
		{
			//Clear any previous screens
			ClearScreen(consoleBuffer);

			//Display menu options
			WriteTextToBuffer(consoleBuffer, "1. Play", (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) - 2);
			WriteTextToBuffer(consoleBuffer, "2. Scoreboard", (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) - 1);
			WriteTextToBuffer(consoleBuffer, "3. Options", (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2));
			WriteTextToBuffer(consoleBuffer, "4. Quit", (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) + 1);
	
			if (GetAsyncKeyState(KEY_1))
			{
				currentGameState = GAME;
			}
			else if (GetAsyncKeyState(KEY_2))
			{
				//Clear screen before entering Scoreboard to save on constant screen clearing while in the Scoreboard
				ClearScreen(consoleBuffer);
				//Move into the Scoreboard
				currentGameState = SCOREBOARD;
			}
			else if (GetAsyncKeyState(KEY_3))
			{
				//Clear screen before entering Options to save on constant screen clearing while in the Options
				ClearScreen(consoleBuffer);
				//Move into the Options
				currentGameState = OPTIONS;
			}
			else if (GetAsyncKeyState(KEY_4))
			{
				exitGame = true;
			}
	
			break;
		}
		case OPTIONS:
		{
			//Return to menu if esc is hit
			if (GetAsyncKeyState(KEY_ESC))
			{
				currentGameState = MAIN_MENU;
			}

			WriteTextToBuffer(consoleBuffer, "Esc. Return to menu", (SCREEN_WIDTH / 2) - 9, (SCREEN_HEIGHT / 2) - 2);
		break;
	}
		case SCOREBOARD:
		{
			//Return to menu if esc is hit
			if (GetAsyncKeyState(KEY_ESC))
			{
				currentGameState = MAIN_MENU;
			}

			WriteTextToBuffer(consoleBuffer, "Esc. Return to menu", (SCREEN_WIDTH / 2) - 9, (SCREEN_HEIGHT / 2) - 2);

		break;
		}
		case GAME:
		{
	
			if (!player.hasLanded && !player.hasCrashed && !gamePaused)
			{
				//Increase ingame time
				player.time += deltaTime;
				//Take the player's input
				if (GetAsyncKeyState(KEY_ESC))
				{
					gamePaused = true;
				}
				if (GetAsyncKeyState(KEY_W))
				{
					//Checks the player has enough fuel to move upwards
					if (player.fuel > 0.0f)
					{
						player.isAccelerating = true;
						player.fuel -= FUEL_CONSUMPTION_RATE;
					}
					player.direction = 'u';
				}
				if (GetAsyncKeyState(KEY_A))
				{
					//Checks the player has enough fuel to move sideways
					if (player.fuel > 0.0f)
					{
						--player.XPos;
						player.fuel -= FUEL_CONSUMPTION_RATE;
					}
					player.direction = 'l';
				}
				if (GetAsyncKeyState(KEY_D))
				{
					//Checks the player has enough fuel to move sideways
					if (player.fuel > 0.0f)
					{
						++player.XPos;
						player.fuel -= FUEL_CONSUMPTION_RATE;
					}
					player.direction = 'r';
				}
	
				//Should we Accelerate?
				if (player.isAccelerating)
				{
					player.Acceleration += (ACCELERATION_RATE * deltaTime);
				}
				else
				{
					player.Acceleration -= (DECELERATION_RATE * deltaTime);
				}
	
				//clamp the Acceleration
				player.Acceleration = ClampFloat(player.Acceleration, 0.0f, 1.5f);

				if (player.Acceleration >= 1.0f) //TODO REMOVE MAGIC NUMBER
				{
					player.YPos--;
				}
				else if (player.Acceleration < 0.5f)
				{
					player.YPos++;
				}

				// Clamp the input
				player.XPos = ClampInt(player.XPos, 0, (SCREEN_WIDTH - player.WIDTH));
				player.YPos = ClampInt(player.YPos, 0, (SCREEN_HEIGHT - player.HEIGHT));

				static char bottomLeftChar;
				static char bottomRightChar;

				//Get the characters under our ship
				switch (player.direction)
				{
					case 'u':
					{
						bottomLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 2))];
						bottomRightChar = background.CHARACTERS[(player.XPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 2))];
						
						//Check if the player crashed or landed
						if (bottomLeftChar == '_' && bottomRightChar == '_')
						{
							player.hasLanded = true;
						}
						else if ((bottomLeftChar != ' ' || bottomRightChar != ' ') && (bottomLeftChar != '*' && bottomRightChar != '*'))
						{
							player.hasCrashed = true;
						}

						break;
					}
					case 'l':
					{
						//Check the player hasn't hit the ground sideways
						bottomLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.ROTATED_HEIGHT - 2))];
						bottomRightChar = background.CHARACTERS[(player.XPos + (player.ROTATED_WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.ROTATED_HEIGHT - 2))];
						
						if ((bottomLeftChar != ' ' || bottomRightChar != ' ') && (bottomLeftChar != '*' && bottomRightChar != '*'))
						{
							player.hasCrashed = true;
						}
						break;
					}
					case 'r':
					{
						//Check the player hasn't hit the ground sideways
						bottomLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.ROTATED_HEIGHT - 2))];
						bottomRightChar = background.CHARACTERS[(player.XPos + (player.ROTATED_WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.ROTATED_HEIGHT - 2))];
						
						if ((bottomLeftChar != ' ' || bottomRightChar != ' ') && (bottomLeftChar != '*' && bottomRightChar != '*'))
						{
							player.hasCrashed = true;
						}
						break;
					}
				}
			}

			//Draw GameOver Screen
			if (player.hasCrashed)
			{
				//Tell the player they've crashed
				WriteTextToBuffer(consoleBuffer, "You Crashed", (SCREEN_WIDTH / 2) - 5, (SCREEN_HEIGHT / 2));
				WriteTextToBuffer(consoleBuffer, "Your score was: " + std::to_string(player.score), (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) + 1);
				WriteTextToBuffer(consoleBuffer, "Enter. Return to the Main Menu", (SCREEN_WIDTH / 2) - 15, (SCREEN_HEIGHT / 2) + 2);
				WriteTextToBuffer(consoleBuffer, "Esc. Quit Game", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2) + 3);

				static float explosionFlashTimer = 0.0f;

				explosionFlashTimer += deltaTime;

				if (explosionFlashTimer >= 0.25f)
				{
					//Draw our explosion
					WriteImageToBuffer(consoleBuffer, explosion.FIRST_CHARACTERS, explosion.FIRST_COLOURS, explosion.HEIGHT, explosion.WIDTH, player.XPos - 1, player.YPos - 1);
					if (explosionFlashTimer >= 0.5f)
					{
						explosionFlashTimer = 0.0f;
					}
				}
				else 
				{
					//Draw next step of explosion
					WriteImageToBuffer(consoleBuffer, explosion.SECOND_CHARACTERS, explosion.SECOND_COLOURS, explosion.HEIGHT, explosion.WIDTH, player.XPos - 1, player.YPos - 1);
				}

				//Exit Game
				if (GetAsyncKeyState(KEY_ESC))
				{
					exitGame = true;
				}
				//Return to the menu
				if (GetAsyncKeyState(KEY_ENTER))
				{
					player.TotalReset();
					currentGameState = MAIN_MENU;
				}
				break;
			}
			//Draw Player landed text
			else if (player.hasLanded)
			{
				//Workout where the player landed
				char scoreChar = background.POINTS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];
				WriteTextToBuffer(consoleBuffer, "The Eagle has Landed", (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2));
				if (!player.hasAddedScore)
				{
					player.hasAddedScore = true;
					player.score = WorkOutScore(player.score, scoreChar);
				}

				if (player.fuel > 0)
				{
					static float respawnTimer = 0.0f;
					respawnTimer += deltaTime;

					WriteTextToBuffer(consoleBuffer, "Relaunching in: " + std::to_string(5.0f - respawnTimer), (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) + 1);
					if (respawnTimer >4.0f)
					{
						player.Reset();
						respawnTimer = 0.0f;
					}
				}
				else 
				{
					//Displays the player's score and options once fuel has run out
					WriteTextToBuffer(consoleBuffer, "Fuel Reserves Empty Return to base", (SCREEN_WIDTH / 2) - 17, (SCREEN_HEIGHT / 2) + 1);
					WriteTextToBuffer(consoleBuffer, "Your score was: " + std::to_string(player.score), (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) + 2);
					WriteTextToBuffer(consoleBuffer, "Enter. Return to menu", (SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) + 3);
					WriteTextToBuffer(consoleBuffer, "Esc. Quit game", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2) + 4);

					if (GetAsyncKeyState(KEY_ENTER))
					{
						player.TotalReset();
						currentGameState = MAIN_MENU;
					}
				}
			}
			//Draw Pause menu
			else if (gamePaused)
			{
				WriteTextToBuffer(consoleBuffer, "1. Resume Game", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2));
				WriteTextToBuffer(consoleBuffer, "2. Quit to Menu", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2) + 1);

				if (GetAsyncKeyState(KEY_1))
				{
					//unpause the game
					gamePaused = false;
				}
				if (GetAsyncKeyState(KEY_2))
				{
					//Reset the player state and unpause the game for when it next loads
					player.TotalReset();
					gamePaused = false;
					//Return to the game's menu
					currentGameState = MAIN_MENU;
				}
			}
			//Draw the Game
			else
			{
				// Clear the previous 'frame' before we start building the next one
				ClearScreen(consoleBuffer);

				//Draw background image
				WriteImageToBuffer(consoleBuffer, background.CHARACTERS, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

				//Draw player in correct direction
				switch (player.direction)
				{
					case 'u':
					{
						//Draws standard upright rocket
						WriteImageToBuffer(consoleBuffer, player.CHARACTERS, player.COLOURS, player.HEIGHT, player.WIDTH, player.XPos, player.YPos);
						if (player.isAccelerating)
						{
							WriteImageToBuffer(consoleBuffer, thrust.CHARACTERS, thrust.COLOURS, thrust.HEIGHT, thrust.WIDTH, player.XPos, player.YPos + player.HEIGHT);
							//Reset Acceleration
							player.isAccelerating = false;
						}
						break;
					}
					case 'l':
					{
						//if the player is rotated left draws the left rotation version of the rocket
						WriteImageToBuffer(consoleBuffer, player.LEFT_CHARACTERS, player.LEFT_COLOURS, player.ROTATED_HEIGHT, player.ROTATED_WIDTH, player.XPos, player.YPos);
						//if the player accelerates adds the booster flames under the sideways rocket
						if (player.isAccelerating)
						{
							WriteImageToBuffer(consoleBuffer, thrust.LEFT_CHARACTERS, thrust.LEFT_COLOURS, thrust.ROTATED_HEIGHT, thrust.ROTATED_WIDTH, player.XPos + player.ROTATED_WIDTH, player.YPos);
							//Reset Acceleration
							player.isAccelerating = false;
						}
						break;
					}
					case 'r':
					{
						//if the player is rotated right draws the right rotation version of the rocket
						WriteImageToBuffer(consoleBuffer, player.RIGHT_CHARACTERS, player.RIGHT_COLOURS, player.ROTATED_HEIGHT, player.ROTATED_WIDTH, player.XPos, player.YPos);
						//if the player accelerates adds the booster flames under the sideways rocket
						if (player.isAccelerating)
						{
							WriteImageToBuffer(consoleBuffer, thrust.RIGHT_CHARACTERS, thrust.RIGHT_COLOURS, thrust.ROTATED_HEIGHT, thrust.ROTATED_WIDTH, player.XPos - (player.ROTATED_WIDTH - 1), player.YPos);
							//Reset Acceleration
							player.isAccelerating = false;
						}
						break;
					}
				}

				//Draw Text UI to screen
				WriteTextToBuffer(consoleBuffer, "SCORE: " + std::to_string(player.score), 0, 0);
				WriteTextToBuffer(consoleBuffer, "TIME: " + std::to_string(player.time) , 0, 1);
				WriteTextToBuffer(consoleBuffer, "FUEL: " + std::to_string(player.fuel), 0, 2);
			}

			break;
		}
	}
}

void Game::Draw()
{
	// Kick the draw
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}