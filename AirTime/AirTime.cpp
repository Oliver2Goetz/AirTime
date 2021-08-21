#include "pch.h"
#include "AirTime.h"


BAKKESMOD_PLUGIN(AirTime, "Shows you how long you stayed in the air after jumping", "1.0", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void AirTime::onLoad() {
	_globalCvarManager = cvarManager;

	// Happens when a player jumps
	gameWrapper->HookEvent("Function CarComponent_Jump_TA.Active.BeginState", bind(&AirTime::onJump, this, std::placeholders::_1));

	//Happen when a player lands on the ground with its wheels
	gameWrapper->HookEvent("Function TAGame.Car_TA.EventLanded", bind(&AirTime::onLandOnGround, this, std::placeholders::_1)); 

	//Register Notifiers
	cvarManager->registerNotifier("start_timer", [this](std::vector<std::string> args) {
		startTimer();
	}, "", PERMISSION_ALL);

	cvarManager->registerNotifier("stop_timer", [this](std::vector<std::string> args) {
		stopTimer();
	}, "", PERMISSION_ALL);

	//Canvas-Rendering
	gameWrapper->RegisterDrawable(std::bind(&AirTime::Render, this, std::placeholders::_1));
}

void AirTime::onUnload() {}

void AirTime::onJump(std::string eventName) {
	startTimer();
}

void AirTime::onLandOnGround(std::string eventName) {
	stopTimer();
}

void AirTime::startTimer() {
	auto start = std::chrono::high_resolution_clock::now();
	start_time = start.time_since_epoch();
	cvarManager->log("[AirTime] Start timer: " + std::to_string(start_time.count()));
}

void AirTime::stopTimer() {
	auto finish = std::chrono::high_resolution_clock::now();
	stop_time = finish.time_since_epoch();
	cvarManager->log("[AirTime] Stop timer: " + std::to_string(stop_time.count()));

	last_diff = stop_time - start_time;
	cvarManager->log("[AirTime] Difference: " + std::to_string(last_diff.count()) + " s");
	initialized = true;
}

void AirTime::Render(CanvasWrapper canvas) {
	if (!initialized) { return; }

	canvas.SetPosition(Vector2F{ 15, 40 });
	canvas.SetColor(LinearColor{ 0, 0, 0, 255 });
	
	canvas.DrawString("Air time: " + std::to_string(last_diff.count()) + " s", 2, 2);
}
