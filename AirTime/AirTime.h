#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
#include <chrono>

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class AirTime: public BakkesMod::Plugin::BakkesModPlugin {

	virtual void onLoad();
	virtual void onUnload();

	void onJump(std::string eventName);
	void onLandOnGround(std::string eventName);

	void Render(CanvasWrapper canvas);

	void startTimer();
	void stopTimer();

	std::chrono::duration<double> start_time;
	std::chrono::duration<double> stop_time;
	std::chrono::duration<double> last_diff;
	bool initialized = false; //Relevant for rendering

};
