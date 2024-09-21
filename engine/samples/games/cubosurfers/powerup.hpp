#pragma once

#include <cubos/engine/prelude.hpp>
#include <glm/vec3.hpp>

enum class PowerUpType
{
    ARMOR, JETPACK
};

CUBOS_REFLECT_EXTERNAL_DECL(CUBOS_EMPTY, PowerUpType);

struct PowerUp
{
    CUBOS_REFLECT;

    PowerUpType type;
    float killZ{-10.0F};
    glm::vec3 velocity{0.0F, 0.0F, -100.0F};
};

void powerUpPlugin(cubos::engine::Cubos& cubos);
