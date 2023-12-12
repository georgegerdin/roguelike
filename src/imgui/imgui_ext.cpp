
#include <algorithm>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_ext.h"

namespace ImGuiExt {

float length_squared(ImVec2 vec) {
    return vec.x * vec.x + vec.y * vec.y;
}

bool between(ImVec2 point, ImVec2 p1, ImVec2 p2) {
    const auto dx = p2.x - p1.x;
    const auto dy = p2.y - p1.y;

    if(std::abs(dx) >= std::abs(dy))
        return dx > 0 ?
               p1.x <= point.x && point.x <= p2.x :
               p2.x <= point.x && point.x <= p1.x;

    return dy > 0 ?
           p1.y <= point.y && point.y <= p2.y :
           p2.y <= point.y && point.y <= p1.y;
}


float calculate_angle(ImVec2 const p1, ImVec2 const p2) {
    auto radians = atan2(p1.y - p2.y, p1.x - p2.x);
    if(radians < 0.0f) radians = (2.f * M_PI) - std::abs(radians);
    radians+= M_PI;
    if(radians > 2.f*M_PI) radians-= 2.f*M_PI;
    return radians;
}

std::vector<Intersection> circle_line_intersection(ImVec2 A, ImVec2 B, ImVec2 C, float radius) {
    const auto p1 = A - C;
    const auto p2 = B - C;

    const auto d = p2 - p1;

    float det = p1.x * p2.y - p2.x * p1.y;
    float dSq = length_squared(d);

    float discriminant = radius * radius * dSq - det * det;

    if(discriminant < 0.1f)
        return {};
    //if(discriminant < 0.01f) {
    //    std::vector<ImVec2> result = {ImVec2{det * d.y / dSq + C.x, -det * d.x / dSq + C.y}};
    //    return {};
    //}

    float discSqrt = sqrt(discriminant);
    float sgn = d.y < 0.f ? -1 : 1;

    std::vector<Intersection> result;
    auto first_intersection = ImVec2( (det * d.y + sgn * d.x * discSqrt) / dSq + C.x, (-det * d.x + std::abs(d.y) * discSqrt) / dSq + C.y);
    if(between(first_intersection, A, B))
        result.emplace_back(Intersection{IntersectionType::In, first_intersection, calculate_angle(C, first_intersection)});
    auto second_intersection = ImVec2((det * d.y - sgn * d.x * discSqrt) / dSq + C.x, (-det * d.x - std::abs(d.y) * discSqrt) / dSq + C.y);
    if(between(second_intersection, A, B))
        result.emplace_back(Intersection{IntersectionType::Out, second_intersection, calculate_angle(C, second_intersection)});
    return result;
}

std::vector<Intersection> circle_rect_intersection(ImRect rect, ImVec2 circle_center, float circle_radius ) {
    auto concat = [](auto& dest, auto src) {
        dest.insert(dest.end(), src.begin(), src.end());
    };

    auto intersections = circle_line_intersection(rect.GetTL(), rect.GetTR(), circle_center, circle_radius);
    concat(intersections, circle_line_intersection(rect.GetTR(), rect.GetBR(), circle_center, circle_radius));
    concat(intersections, circle_line_intersection(rect.GetBR(), rect.GetBL(), circle_center, circle_radius));

    // Fix intersection order on the left side
    auto left_intersections = circle_line_intersection(rect.GetBL(), rect.GetTL(), circle_center, circle_radius);
    for(auto& i : left_intersections) {
        if(i.type == IntersectionType::In)
            i.type = IntersectionType::Out;
        else
            i.type = IntersectionType::In;
    }
    concat(intersections, left_intersections);

    std::sort(intersections.begin(), intersections.end(),
              [] (auto const& a, auto const& b) {
                  return a.rad_angle < b.rad_angle;
              }
    );

    return intersections;
}

// Draws a circle with the ability to specify angle min and angle max
void AddCircle(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness, float a_min = 0.0f, float a_max = IM_PI * 2.f)
{
    if ((col & IM_COL32_A_MASK) == 0 || radius <= 0.0f)
        return;

    // Obtain segment count
    if (num_segments <= 0)
    {
        // Automatic segment count
        num_segments = draw_list->_CalcCircleAutoSegmentCount(radius);
    }
    else
    {
        // Explicit segment count (still clamp to avoid drawing insanely tessellated shapes)
        num_segments = ImClamp(num_segments, 3, IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);
    }

    draw_list->PathArcTo(center, radius - 0.5f, a_min, a_max, num_segments);
    draw_list->PathStroke(col, ImDrawFlags_None, thickness);
}

// Draws a filled circle with the ability to specify angle min and angle max
void AddCircleFilled(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col, float a_min, float a_max)
{
    if ((col & IM_COL32_A_MASK) == 0 || radius <= 0.0f)
        return;

    auto const num_segments = draw_list->_CalcCircleAutoSegmentCount(radius);


    draw_list->PathLineTo(center);
    draw_list->PathArcTo(center, radius, a_min, a_max, num_segments);
    draw_list->PathFillConvex(col);
}

float distance(ImVec2 a, ImVec2 b) {
    auto x = a.x - b.x;
    auto y = a.y - b.y;
    return sqrt(x*x + y*y);
}

void AddClippedCircle(ImDrawList* draw_list, ImRect rect, ImVec2 circle_center, float circle_radius, ImU32 color, int num_segments, float thickness) {
    auto intersections = circle_rect_intersection(rect, circle_center, circle_radius);

    if(intersections.size() == 0) {
        // Special case when the circle does not intersect the clip rect
        if(!rect.Contains(circle_center))
            return; // Circle is completely outside the clip rect if the circle center is outside and there are no intersections

        // Special case where there are no intersections but the circle center is inside the clip rect
        if(std::abs(distance(rect.Min, circle_center)) < circle_radius)
            return; // The whole circle is outside the clip rect even though the center is inside

        // The whole circle is inside the clip rect
        AddCircle(draw_list, circle_center, circle_radius, color, num_segments, thickness, 0, 2.0f * M_PI);
        return;
    }

    // Draw circle segment for each intersection
    for(auto n = 0; n < intersections.size(); ++n) {
        auto const& i = intersections[n];
        if(intersections[n].type == IntersectionType::In) {
            float a_max = 2.0f * M_PI;
            if(n + 1 < intersections.size()) {
                a_max = intersections[n + 1].rad_angle;
            }
            else {
                a_max+= intersections[0].rad_angle; // Overflow 360+ degrees (2.0f * M_PI radians)
            }

            AddCircle(draw_list, circle_center, circle_radius, color, num_segments, thickness, i.rad_angle, a_max);
        }
    }
}

}