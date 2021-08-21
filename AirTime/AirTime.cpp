#include "pch.h"
#include "AirTime.h"


BAKKESMOD_PLUGIN(AirTime, "Shows you how long you stayed in the air after jumping", "1.0", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void AirTime::onLoad() {
	_globalCvarManager = cvarManager;

	cvarManager->registerCvar("airtime_enabled", "0", "AirTime enabled", true, true, 0, true, 1);

	// Happens when a player jumps
	gameWrapper->HookEvent("Function CarComponent_Jump_TA.Active.BeginState", bind(&AirTime::onJump, this, std::placeholders::_1));

	//Happen when a player lands on the ground with its wheels
	gameWrapper->HookEvent("Function TAGame.Car_TA.EventLanded", bind(&AirTime::onLandOnGround, this, std::placeholders::_1)); 

	//Register Notifiers
	cvarManager->registerNotifier("airtime_start_timer", [this](std::vector<std::string> args) {
		startTimer();
	}, "", PERMISSION_ALL);

	cvarManager->registerNotifier("airtime_stop_timer", [this](std::vector<std::string> args) {
		stopTimer();
	}, "", PERMISSION_ALL);

	cvarManager->registerNotifier("airtime_clear_average", [this](std::vector<std::string> args) {
		clearAverage();
	}, "", PERMISSION_ALL);

	//Canvas-Rendering
	gameWrapper->RegisterDrawable(std::bind(&AirTime::Render, this, std::placeholders::_1));
}

void AirTime::onUnload() {}

void AirTime::onJump(std::string eventName) {
	CVarWrapper enableCvar = cvarManager->getCvar("airtime_enabled");
	if (!enableCvar) { return; }

	bool enabled = enableCvar.getBoolValue();
	if (enabled) {
		startTimer();
	}
}

void AirTime::onLandOnGround(std::string eventName) {
	CVarWrapper enableCvar = cvarManager->getCvar("airtime_enabled");
	if (!enableCvar) { return; }

	bool enabled = enableCvar.getBoolValue();
	if (enabled) {
		stopTimer();
	}
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

	all_times.push_back(last_diff.count());
	jump_times++;
	calculateAverageTime();
}

void AirTime::calculateAverageTime() {
	double sum = 0.0;
	for (int i = 0; i < all_times.size(); i++) {
		sum += all_times.at(i);
	}

	average_time = sum / all_times.size();
}

void AirTime::clearAverage() {
	all_times.clear();
	average_time = 0.0;
	jump_times = 0;
}

void AirTime::Render(CanvasWrapper canvas) {
	if (!initialized) { return; }

	canvas.SetColor(LinearColor{ 0, 0, 0, 255 });

	//Draw single time
	canvas.SetPosition(Vector2F{ 15, 40 });
	canvas.DrawString("Air time: " + std::to_string(last_diff.count()) + " s", 2, 2);
	//Draw average time
	canvas.SetPosition(Vector2F{ 15, 70 });
	canvas.DrawString("Average: " + std::to_string(average_time) + " s (" + std::to_string(jump_times) + ")", 2, 2);
}
