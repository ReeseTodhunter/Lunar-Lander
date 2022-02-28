// INCLUDES
#include <windows.h>
#include <chrono>
#include "Game.h"

// TYPEDEFS
typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HiResClock;
typedef std::chrono::duration<float> TimeDiff;

int main()
{
	Game gameInstance;

	//initialise our console window
	gameInstance.Intitialise();

	//initialise variables
	float deltaTime = 0.0f;
	Time currentFrameTime = HiResClock::now();
	Time previousFrameTime = HiResClock::now();

	// Main game loop
	while (!gameInstance.exitGame)
	{
		// Calculate our delta time (time since last frame)
		currentFrameTime = HiResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime;
		deltaTime = diff.count();

		if (deltaTime >= (1.0f / FRAME_RATE))
		{
			//Update Application
			gameInstance.Update(deltaTime);

			// Cache the timestamp of this frame
			previousFrameTime = currentFrameTime;
		}

		gameInstance.Draw();
	}

	return 0;
}