//
// Created by george on 2023-12-03.
//
#include <cstdio>
#include "entities.hh"

const char* entity_type(Entities::Entity const& e) {
    return std::visit(
        [](auto const& e) -> const char* {
            return e.type();
        },
        e
    );
}

int main(int argc, char* argv[]) {

    Entities::Entity entity = Entities::Player{};
    printf("Created an entity: %s\n", entity_type(entity));

    Entities::Entity entity2 = Entities::Flower{};
    printf("Created another entity: %s\n", entity_type(entity2));
    return 0;
}