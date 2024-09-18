#pragma once

#include <cubos/engine/assets/asset.hpp>
#include <cubos/engine/prelude.hpp>
#include <cubos/engine/scene/scene.hpp>

struct AccumulatedTime
{
    CUBOS_REFLECT;

    float time{0.0F};
};

void accumulatedTimePlugin(cubos::engine::Cubos& cubos);
