#include "stdafx.h"

#include "Profiler.h"

namespace DentyEngine
{
    void Profiler::AddSystem(std::string_view name, double microseconds)
    {
        _systemMeasuredData.emplace(name, microseconds);
    }

    void Profiler::AddGraphics(std::string_view name, double microseconds)
    {
        _graphicsMeasuredData.emplace(name, microseconds);
    }

    void Profiler::OnGui()
    {
        // System
        if (ImGui::TreeNodeEx("System##Profiler"))
        {
            // Display all system measured data.
            for (const auto& measuredDatum : _systemMeasuredData)
            {
                // Name
                ImGui::Text("Name: ");
                ImGui::SameLine();
                ImGui::Text(measuredDatum.first.c_str());

                // Time (ms)
                ImGui::Indent();

                ImGui::Text("Time(ms): ");
                ImGui::SameLine();
                ImGui::Text("%lf", measuredDatum.second);

                ImGui::Unindent();
            }

            ImGui::TreePop();
        }

        // Graphics
        if (ImGui::TreeNodeEx("Graphics##Profiler"))
        {
            // Display all graphics measured data.
            for (const auto& measuredDatum : _graphicsMeasuredData)
            {
                // Name
                ImGui::Text("Name: ");
                ImGui::SameLine();
                ImGui::Text(measuredDatum.first.c_str());

                // Time (ms)
                ImGui::Indent();

                ImGui::Text("Time(ms): ");
                ImGui::SameLine();
                ImGui::Text("%lf", measuredDatum.second);

                ImGui::Unindent();
            }

            ImGui::TreePop();
        }
    }

    void Profiler::Clear()
    {
        _systemMeasuredData.clear();
        _graphicsMeasuredData.clear();
    }
}
