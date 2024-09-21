#include "obstacle.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include "accumulated_time.hpp"
#include "score.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Obstacle)
{
    return cubos::core::ecs::TypeBuilder<Obstacle>("Obstacle")
        .withField("velocity", &Obstacle::velocity)
        .withField("killZ", &Obstacle::killZ)
        .build();
}

void obstaclePlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(assetsPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(accumulatedTimePlugin);
    cubos.depends(scorePlugin);

    cubos.component<Obstacle>();

    cubos.system("move obstacles")
        .call([](Commands cmds, const DeltaTime& dt, Score& score, const AccumulatedTime& at, Query<Entity, const Obstacle&, Position&> obstacles) {
            for (auto [ent, obstacle, position] : obstacles)
            {
                glm::vec3 velocity = obstacle.velocity + obstacle.velocity * at.time / 120.0F;
                position.vec += velocity * dt.value();
                position.vec.y = glm::abs(glm::sin(position.vec.z * 0.15F)) * 1.5F;

                if (position.vec.z < obstacle.killZ)
                {
                    score.score++;
                    cmds.destroy(ent);
                }
            }
        });
}
