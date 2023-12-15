//
// Created by george on 2023-12-03.
//
#include <cstdio>
#include <imgui/imgui.h>
#include <array>
#include <string>
#include "entities.hh"

const char* entity_type(Entities::Entity const& e) {
    return std::visit(
        [](auto const& e) -> const char* {
            return e.type();
        },
        e
    );
}

struct Position {
    int x, y;
};

Position entity_position(Entities::Entity const& e) {
    return std::visit(
            [](auto const& e) -> Position {
                Position pos;
                e.position(&pos.x, &pos.y);
                return pos;
            },
            e
    );
}

void entity_set_position(Entities::Entity& e, Position pos) {
    std::visit(
        [pos](auto& e) {
            e.set_position(pos.x, pos.y);
        },
        e
    );
}

char entity_symbol(Entities::Entity const& e) {
    return std::visit(
            [](auto const& e) -> char {
                return e.symbol();
            },
            e
    );
}

class RenderFrame {
    static const int WIDTH = 10, HEIGHT = 10;
    std::array<char, WIDTH * HEIGHT> m_frame;
public:
    RenderFrame() {
        for(char& tile : m_frame) {
            tile = ' ';
        }
    }

    void set(int x, int y, char c) {
        m_frame[ (WIDTH*y) + x] = c;
    }

    char get(int x, int y) const {
        return m_frame[ (WIDTH * y) + x];
    }

    int width() const { return WIDTH; }
    int height() const { return HEIGHT; }
};


bool imgui_init();
void imgui_start_frame();
bool imgui_end_frame();
void imgui_shutdown();

void render_entity(Entities::Entity const& ent, RenderFrame& frame) {
    auto pos = entity_position(ent);
    frame.set(pos.x, pos.y, entity_symbol(ent));
}

void render_frame(RenderFrame const& frame) {
    for(auto y = 0; y < frame.width(); ++y) {
        std::string s;
        for(auto x = 0; x < frame.width(); ++x)
            s+= frame.get(x, y);
        ImGui::Text("%s", s.c_str());
    }
}

int main(int argc, char* argv[]) {

    Entities::Entity entity = Entities::Player{};
    entity_set_position(entity, Position{3, 5});
    printf("Created an entity: %s\n", entity_type(entity));

    Entities::Entity entity2 = Entities::Flower{};
    printf("Created another entity: %s\n", entity_type(entity2));

    if (!imgui_init()) return 0;

    bool done = false;
    const int frame_width = 200, frame_height = 200;
    RenderFrame frame;
    while(!done) {
        imgui_start_frame();

        ImGui::Begin("Image", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Hello!");
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
        ImGui::BeginChild("world", ImVec2(frame_width, frame_height), true);

        render_entity(entity, frame);
        render_entity(entity2, frame);
        render_frame(frame);

        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::End();
        done = !imgui_end_frame();
    }


    imgui_shutdown();
    return 0;
}