#include "powerup.hpp"

#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/voxels/grid.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/reflect.hpp>
#include <cubos/core/reflection/traits/enum.hpp>
#include <cubos/core/reflection/type.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include "accumulated_time.hpp"
#include "player.hpp"
#include "armor.hpp"
#include "jetpack.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_EXTERNAL_IMPL(PowerUpType)
{
    return Type::create("PowerUpType").with(
        EnumTrait{}.withVariant<PowerUpType::ARMOR>("Armor").withVariant<PowerUpType::JETPACK>("Jetpack"));
}

CUBOS_REFLECT_IMPL(PowerUp)
{
    return cubos::core::ecs::TypeBuilder<PowerUp>("PowerUp")
        .withField("type", &PowerUp::type)
        .withField("velocity", &PowerUp::velocity)
        .withField("killZ", &PowerUp::killZ)
        .build();
}

void powerUpPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(playerPlugin);
    cubos.depends(collisionsPlugin);
    cubos.depends(armorPlugin);
    cubos.depends(jetpackPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(accumulatedTimePlugin);

    cubos.component<PowerUp>();

    cubos.system("powerup move system")
        .call([](Commands cmds, const DeltaTime& dt, const AccumulatedTime& at, Query<Entity, const PowerUp&, Position&> obstacles) {
            for (auto [ent, powerUp, position] : obstacles)
            {
                glm::vec3 velocity = powerUp.velocity + powerUp.velocity * at.time / 120.0F;
                position.vec += velocity * dt.value();

                if (position.vec.z < powerUp.killZ)
                {
                    cmds.destroy(ent);
                }
            }
        });

    cubos.system("powerup pickup system")
        .with<Player>()
        .without<Armor>()
        .without<Jetpack>()
        .call([](Commands cmds, Query<Entity, const CollidingWith&, const PowerUp&, Entity> query) {
            for (auto [player, collidingWith, powerUp, entity] : query)
            {
                cmds.destroy(entity);
                switch (powerUp.type)
                {
                    case PowerUpType::ARMOR:
                        cmds.add(player, Armor{});
                        break;
                    
                    case PowerUpType::JETPACK:
                        cmds.add(player, Jetpack{});
                        break;
                    default:
                        CUBOS_ERROR("Powerup type not in switch case!");
                        break;
                }
            }
        });
}
