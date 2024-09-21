#pragma once

#include <cubos/engine/prelude.hpp>
#include <cstdint>

struct Score
{
    CUBOS_REFLECT;

    uint32_t score{0};
};

void scorePlugin(cubos::engine::Cubos& cubos);
