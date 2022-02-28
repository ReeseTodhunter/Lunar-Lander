#ifndef UTILITY_H
#define UTILITY_H

//Includes
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Constants.h"

static int ClampInt(int intToClamp, int lowerLimit, int upperLimit)
{
	if (intToClamp <= lowerLimit)
	{
		return lowerLimit;
	}
	else if (intToClamp >= upperLimit)
	{
		return upperLimit;
	}
	else
	{
		return intToClamp;
	}
}

static float ClampFloat(float floatToClamp, float lowerLimit, float upperLimit)
{
	return floatToClamp <= lowerLimit ? lowerLimit : floatToClamp >= upperLimit ? upperLimit : floatToClamp;
}

static void WriteImageToBuffer(CHAR_INFO* consoleBuffer, const char* charsToPrint, const int coloursToPrint[], int imageHeight, int imageWidth, int imageXPos, int imageYPos)
{
	for (int y = 0; y < imageHeight; ++y)
	{
		for (int x = 0; x < imageWidth; ++x)
		{
			consoleBuffer[(imageXPos + x) + SCREEN_WIDTH * (imageYPos + y)].Char.AsciiChar = charsToPrint[x + imageWidth * y];
			if (coloursToPrint != nullptr)
			{
				consoleBuffer[(imageXPos + x) + SCREEN_WIDTH * (imageYPos + y)].Attributes = coloursToPrint[x + imageWidth * y];
			}
			else
			{
				consoleBuffer[(imageXPos + x) + SCREEN_WIDTH * (imageYPos + y)].Attributes = 7;
			}
		}
	}
}

static void ClearScreen(CHAR_INFO* consoleBuffer)
{
	for (int i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); ++i)
	{
		consoleBuffer[i].Char.AsciiChar = 0;
		consoleBuffer[i].Attributes = 0;
	}
}

static void WriteTextToBuffer(CHAR_INFO* consoleBuffer, std::string stringToPrint, int textXPos, int textYPos)
{
	stringToPrint = stringToPrint;

	for (int x = 0; x < stringToPrint.length(); x++)
	{
		consoleBuffer[(textXPos + x) + SCREEN_WIDTH * textYPos].Char.AsciiChar = stringToPrint[x];
		consoleBuffer[(textXPos + x) + SCREEN_WIDTH * textYPos].Attributes = 0xF;
	}
}

static int WorkOutScore(int score, char landingpad)
{
	switch (landingpad)
	{
		case '1':
		{
			score += SCORE_INCREASE;
			break;
		}
		case '2':
		{
			score += (SCORE_INCREASE * 2);
			break;
		}
		case '3':
		{
			score += (SCORE_INCREASE * 3);
			break;
		}
		case '4':
		{
			score += (SCORE_INCREASE * 4);
			break;
		}
		case '5':
		{
			score += (SCORE_INCREASE * 5);
			break;
		}
		case '6':
		{
			score += (SCORE_INCREASE * 6);
			break;
		}
	}
	return (score);
}

static std::string ReadFromHighScore(int scoreNumber)
{
	std::string Scores[5];
	int i = 0;
	std::ifstream myfile("HighScores.txt");
	if (myfile.is_open())
	{
		while (std::getline(myfile, Scores[i]))
		{
			++i;
		}
		myfile.close();
	}

	return Scores[scoreNumber];
}
#endif //UTILITY_H
