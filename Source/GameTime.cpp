#include "GameTime.h"

bool GameTime::isStopped = true;
bool GameTime::isPaused = false;

float GameTime::frameCount = 0;
float GameTime::timeScale = 1;
float GameTime::GameTimeSinceStart = 0;
float GameTime::GameTimeDeltaTime = 0;
float GameTime::deltaTime = 0;

void GameTime::Play()
{
	isStopped = false;
}

void GameTime::Stop()
{
	isStopped = true;
	isPaused = false;

	frameCount = 0;
	timeScale = 0;
	GameTimeSinceStart = 0;
	GameTimeDeltaTime = 0;
}

void GameTime::Pause()
{
	isPaused = true;
}

void GameTime::Unpause()
{
	isPaused = false;
}

void GameTime::Tick(float dt)
{
	if (!isStopped)
	{
		deltaTime = dt;
		frameCount++;
		GameTimeDeltaTime = deltaTime;
		GameTimeSinceStart += deltaTime;
	}
}

void GameTime::Update(float dt)
{
	if (!isStopped && !isPaused)
	{
		deltaTime = dt;
		frameCount++;
		GameTimeDeltaTime = deltaTime;
		GameTimeSinceStart += deltaTime;
	}
	else deltaTime = 0;
}

bool GameTime::IsStopped() { return isStopped; }
bool GameTime::IsPaused() { return isPaused; }

float GameTime::GetFrameCount() { return frameCount; }
float GameTime::GetTimeScale() { return timeScale; }
float GameTime::GetGameTimeSinceStart() { return GameTimeSinceStart; }
float GameTime::GetGameTimeDeltaTime() { return GameTimeDeltaTime; }
float GameTime::GetDeltaTime() { return deltaTime; }