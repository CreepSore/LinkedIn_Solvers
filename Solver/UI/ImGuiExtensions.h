#pragma once
#include "imgui.h"

namespace ImGui {
    namespace Extension
    {
        inline bool ColoredButton(const char* label, const ImVec4& color, const ImVec2& size = ImVec2(0,0)) {
            ImVec4 colorHSV(0, 0, 0, 1);

            ColorConvertRGBtoHSV(
                color.x, color.y, color.z,
                colorHSV.x, colorHSV.y, colorHSV.z
            );

            ImVec4 colorActive(0, 0, 0, color.w);

            ColorConvertHSVtoRGB(
                colorHSV.x, colorHSV.y, colorHSV.z * 0.7f,
                colorActive.x, colorActive.y, colorActive.z
            );

            PushStyleColor(ImGuiCol_Button, ImVec4(color.x, color.y, color.z, color.w * 0.6f));
            PushStyleColor(ImGuiCol_ButtonActive, colorActive);
            PushStyleColor(ImGuiCol_ButtonHovered, color);

            const bool result = Button(label, size);

            PopStyleColor(3);

            return result;
        }
    }
}
