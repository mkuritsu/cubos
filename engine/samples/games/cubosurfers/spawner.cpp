#include "spawner.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>
#include <cubos/engine/voxels/grid.hpp>
#include <cubos/engine/render/voxels/plugin.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/engine/collisions/shapes/box.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/engine/collisions/collider.hpp>

#include "powerup.hpp"
#include "obstacle.hpp"

using namespace cubos::engine;

static const Asset<VoxelGrid> ArmorPowerUpAsset = AnyAsset("fd89da0f-72d2-4ef9-9297-f73ff2ca5c63");
static const Asset<VoxelGrid> JetpackPowerUpAsset = AnyAsset("732498a6-7675-4efa-86ef-8f35d3bec485");

CUBOS_REFLECT_IMPL(Spawner)
{
    return cubos::core::ecs::TypeBuilder<Spawner>("Spawner")
        .withField("scene", &Spawner::scene)
        .withField("sceneRoot", &Spawner::sceneRoot)
        .withField("period", &Spawner::period)
        .withField("laneWidth", &Spawner::laneWidth)
        .withField("accumulator", &Spawner::accumulator)
        .build();
}

void spawnerPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(assetsPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(powerUpPlugin);
    cubos.depends(renderVoxelsPlugin);
    cubos.depends(obstaclePlugin);
    cubos.depends(collisionsPlugin);

    cubos.component<Spawner>();

    cubos.system("spawn stuff")
        .call([](Commands commands, const DeltaTime& dt, Assets& assets, Query<Spawner&, const Position&> spawners) {
            for (auto [spawner, position] : spawners)
            {
                spawner.accumulator += dt.value();
                spawner.powerUpAccumulator += dt.value();
                if (spawner.powerUpAccumulator >= spawner.powerUpPeriod)
                {
                    spawner.powerUpAccumulator -= spawner.powerUpPeriod;
                    spawner.accumulator -= spawner.period;
                    
                    Position spawnPosition = position;
                    int offset = (rand() % 3) - 1;
                    spawnPosition.vec.x += static_cast<float>(offset) * spawner.laneWidth;
                    PowerUpType powerUpType = static_cast<PowerUpType>(rand() % 2);

                    auto powerUpEntity = commands.create()
                        .add(spawnPosition)
                        .add(PowerUp{powerUpType})
                        .add(BoxCollisionShape{cubos::core::geom::Box{glm::vec3{3.0F, 7.0F, 2.0F}}})
                        .add(Collider{})
                        .entity();
                    
                    switch (powerUpType)
                    {
                    case PowerUpType::ARMOR:
                        commands.add(powerUpEntity, RenderVoxelGrid{ArmorPowerUpAsset, glm::vec3{-4.0f, 0.0f, 0.0f}});
                        break;
                    case PowerUpType::JETPACK:
                        commands.add(powerUpEntity, RenderVoxelGrid{JetpackPowerUpAsset, glm::vec3{-2.0f, 0.0f, 0.0f}});
                        break;
                    default:
                        break;
                    }
                }
                if (spawner.accumulator >= spawner.period)
                {
                    spawner.accumulator -= spawner.period;

                    Position spawnPosition = position;
                    int offset = (rand() % 3) - 1;
                    spawnPosition.vec.x += static_cast<float>(offset) * spawner.laneWidth;

                    commands.spawn(assets.read(spawner.scene)->blueprint).add(spawner.sceneRoot, spawnPosition);
                }
            }
        });
}
