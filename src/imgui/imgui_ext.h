//
// Created by george on 2022-07-17.
//

#ifndef LIBWEB_IMGUI_EXT_H
#define LIBWEB_IMGUI_EXT_H

#include <vector>

namespace ImGuiExt {

enum IntersectionType {
    In,
    Out
};

struct Intersection {
    IntersectionType type;
    ImVec2 vec;
    float rad_angle;
};

std::vector<Intersection> circle_rect_intersection(ImRect rect, ImVec2 circle_center, float circle_radius );
void AddCircleFilled(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col, float a_min = 0.0f, float a_max = 2.0f*M_PI);
void AddClippedCircle(ImDrawList* draw_list, ImRect rect, ImVec2 circle_center, float circle_radius, ImU32 color, int num_segments = 0, float thickness = 1.0f);

}


#endif //LIBWEB_IMGUI_EXT_H
