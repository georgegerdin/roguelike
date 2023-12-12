//
// Created by george on 2023-12-03.
//
#include <cstdio>
#include <imgui/imgui.h>
#include "entities.hh"

const char* entity_type(Entities::Entity const& e) {
    return std::visit(
        [](auto const& e) -> const char* {
            return e.type();
        },
        e
    );
}

bool imgui_init();
void imgui_start_frame();
bool imgui_end_frame();
void imgui_shutdown();

int main(int argc, char* argv[]) {

    Entities::Entity entity = Entities::Player{};
    printf("Created an entity: %s\n", entity_type(entity));

    Entities::Entity entity2 = Entities::Flower{};
    printf("Created another entity: %s\n", entity_type(entity2));

    if (!imgui_init()) return 0;

    bool done = false;
    while(!done) {
        imgui_start_frame();

        ImGui::Begin("Image", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Hello!");
        ImGui::End();
        done = !imgui_end_frame();
    }


    imgui_shutdown();
    return 0;
}