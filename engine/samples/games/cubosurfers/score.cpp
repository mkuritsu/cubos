#include "score.hpp"

#include <cubos/core/ecs/reflection.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Score)
{
    return cubos::core::ecs::TypeBuilder<Score>("Score")
        .withField("scene", &Score::score)
        .build(); 
}

void scorePlugin(cubos::engine::Cubos& cubos)
{
    cubos.resource<Score>();
}
