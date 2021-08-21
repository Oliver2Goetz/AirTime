#include "pch.h"
#include "AirTime.h"

void AirTime::SetImGuiContext(uintptr_t ctx) {
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string AirTime::GetPluginName() {
    return "AirTime";
}

void AirTime::RenderSettings() {
    //Enable Checkbox
    CVarWrapper enableCvar = cvarManager->getCvar("airtime_enabled");
    if (!enableCvar) { return; }

    bool enabled = enableCvar.getBoolValue();
    if (ImGui::Checkbox("Enable plugin", &enabled)) {
        enableCvar.setValue(enabled);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Toggle AirTime Plugin");
    }

    //Clear Average Button
    if (ImGui::Button("Clear average time")) {
        cvarManager->executeCommand("airtime_clear_average");
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Clear the average time");
    }
}