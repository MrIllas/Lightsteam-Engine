#pragma once

#include "Globals.h"

struct GameTime
{
	static void Play();
	static void Stop();

	static void Pause();
	static void Unpause();

	static void Tick(float dt);
	static void Update(float dt);

	static bool IsStopped();
	static bool IsPaused();

	static float GetFrameCount();
	static float GetTimeScale();
	static float GetGameTimeSinceStart();
	static float GetGameTimeDeltaTime();
	static float GetDeltaTime();
private:
	//Flags
	static bool isStopped;
	static bool isPaused;

	static float frameCount;
	static float timeScale;
	static float GameTimeSinceStart;
	static float GameTimeDeltaTime;
	static float deltaTime;
};