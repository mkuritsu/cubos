#include "jetpack.hpp"

#include <cubos/engine/transform/plugin.hpp>
#include <cubos/core/ecs/reflection.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/render/voxels/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>
#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cmath>

#include "player.hpp"

using namespace cubos::engine;

static const Asset<VoxelGrid> PlayerBaseAsset = AnyAsset("57d1b886-8543-4b8b-8f78-d911e9c4f896");
static const Asset<VoxelGrid> PlayerJetpackAsset = AnyAsset("c7263b46-be18-47c2-b3ef-05592b2e9dec");

CUBOS_REFLECT_IMPL(Jetpack)
{
    return cubos::core::ecs::TypeBuilder<Jetpack>("Jetpack")
        .withField("duration", &Jetpack::duration)
        .build();
}

static const float upAmount = 20.0f;
static const float verticalSpeed = 3.0f;

void jetpackPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(playerPlugin);
    cubos.depends(renderVoxelsPlugin);
    cubos.depends(transformPlugin);

    cubos.component<Jetpack>();

    cubos.observer("add jetpack to player")
        .onAdd<Jetpack>()
        .with<Player>()
        .call([](Commands cmds, Query<Entity, Player&, const RenderVoxelGrid&> query) {
            for (auto [entity, player, voxelGrid] : query)
            {
                cmds.remove<RenderVoxelGrid>(entity);
                cmds.add(entity, RenderVoxelGrid{PlayerJetpackAsset, voxelGrid.offset});
            }
        });

    cubos.system("increase offsetY")
        .with<Jetpack>()
        .call([](Query<Player&> query, const DeltaTime& dt) {
            for (auto [player] : query)
            {
                if (player.offsetY < upAmount)
                {
                    player.offsetY += dt.value() * upAmount * verticalSpeed;
                    player.offsetY = std::min(player.offsetY, upAmount);
                }
            }
        });

    cubos.system("decrease offsetY")
        .without<Jetpack>()
        .call([](Query<Player&> query, const DeltaTime& dt) {
            for (auto [player] : query)
            {
                if (player.offsetY > 0.0f)
                {
                    player.offsetY -= dt.value() * upAmount * verticalSpeed;
                    player.offsetY = std::max(player.offsetY, 0.0f);
                }
            }
        });


    cubos.system("Jetpack system")
        .with<Player>()
        .call([](Commands cmds, Query<Entity, Player&, Jetpack&, const RenderVoxelGrid&> query, const DeltaTime& dt) {
            for (auto [entity, player, jetpack, voxelGrid] : query)
            {
                jetpack.duration -= dt.value();
                if (jetpack.duration <= 0.0f)
                {
                    cmds.remove<Jetpack>(entity);
                    cmds.remove<RenderVoxelGrid>(entity);
                    cmds.add(entity, RenderVoxelGrid{PlayerBaseAsset, voxelGrid.offset});
                }
            }
        });
}
