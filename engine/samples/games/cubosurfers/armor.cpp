#include "armor.hpp"

#include <cubos/engine/transform/plugin.hpp>
#include <cubos/core/ecs/reflection.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/render/voxels/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>
#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/core/reflection/external/glm.hpp>

#include "obstacle.hpp"
#include "player.hpp"

using namespace cubos::engine;

static const Asset<VoxelGrid> PlayerBaseAsset = AnyAsset("57d1b886-8543-4b8b-8f78-d911e9c4f896");
static const Asset<VoxelGrid> PlayerArmorAsset = AnyAsset("4892c2f3-10b3-4ca7-9de3-822b77a0ba7e");

CUBOS_REFLECT_IMPL(Armor)
{
    return cubos::core::ecs::TypeBuilder<Armor>("Armor").build();
}

void armorPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(playerPlugin);
    cubos.depends(obstaclePlugin);
    cubos.depends(transformPlugin);
    cubos.depends(voxelsPlugin);
    cubos.depends(renderVoxelsPlugin);
    cubos.depends(collisionsPlugin);

    cubos.component<Armor>();

    cubos.observer("add child armor to player")
        .onAdd<Armor>()
        .with<Player>()
        .call([](Commands cmds, Query<Entity, const RenderVoxelGrid&> query) {
            for (auto [entity, voxelGrid] : query)
            {
                cmds.remove<RenderVoxelGrid>(entity);
                cmds.add(entity, RenderVoxelGrid{PlayerArmorAsset, voxelGrid.offset});
            }
        });

    cubos.system("player obstacle collision with armor")
        .with<Armor>()
        .with<Player>()
        .call([](Commands cmds, Query<Entity, const RenderVoxelGrid&, const CollidingWith&, const Obstacle&, Entity> query) {
            for (auto [player, grid, collidingWith, obstacle, obstacleEntity] : query)
            {
                cmds.remove<Armor>(player);
                cmds.destroy(obstacleEntity);
                cmds.remove<RenderVoxelGrid>(player);
                cmds.add(player, RenderVoxelGrid{PlayerBaseAsset, grid.offset});
            }
        });
}
