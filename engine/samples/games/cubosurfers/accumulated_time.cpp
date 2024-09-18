#include "accumulated_time.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(AccumulatedTime)
{
    return cubos::core::ecs::TypeBuilder<AccumulatedTime>("AccumulatedTime")
        .withField("time", &AccumulatedTime::time)
        .build();
}

void accumulatedTimePlugin(cubos::engine::Cubos& cubos)
{
    cubos.resource<AccumulatedTime>();

    cubos.system("AccumulatedTime counter").call([](AccumulatedTime& at, const DeltaTime& dt) {
        at.time += dt.value();
    });
}
