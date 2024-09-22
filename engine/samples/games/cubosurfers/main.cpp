#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/defaults/plugin.hpp>
#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/render/lights/environment.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/scene/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

#include "accumulated_time.hpp"
#include "armor.hpp"
#include "obstacle.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "score.hpp"
#include "spawner.hpp"
#include "jetpack.hpp"
#include "gui.hpp"

using namespace cubos::engine;

static const Asset<Scene> SceneAsset = AnyAsset("ee5bb451-05b7-430f-a641-a746f7009eef");
static const Asset<VoxelPalette> PaletteAsset = AnyAsset("101da567-3d23-46ae-a391-c10ec00e8718");
static const Asset<InputBindings> InputBindingsAsset = AnyAsset("b20900a4-20ee-4caa-8830-14585050bead");

static void resetGame(Commands& cmds, AccumulatedTime& at, const Assets& assets, Score& score, Query<Entity> all)
{
    for (auto [ent] : all)
    {
        cmds.destroy(ent);
    }
    at.time = 0.0F;
    score.score = 0;
    cmds.spawn(assets.read(SceneAsset)->blueprint);
}

int main()
{
    Cubos cubos{};

    cubos.plugin(defaultsPlugin);
    cubos.plugin(accumulatedTimePlugin);
    cubos.plugin(scorePlugin);
    cubos.plugin(obstaclePlugin);
    cubos.plugin(playerPlugin);
    cubos.plugin(armorPlugin);
    cubos.plugin(jetpackPlugin);
    cubos.plugin(powerUpPlugin);
    cubos.plugin(spawnerPlugin);
    cubos.plugin(guiPlugin);

    cubos.startupSystem("configure settings").tagged(settingsTag).call([](Settings& settings) {
        settings.setString("assets.io.path", SAMPLE_ASSETS_FOLDER);
    });

    cubos.startupSystem("set the palette, environment, input bindings and spawn the scene")
        .tagged(assetsTag)
        .call([](Commands commands, const Assets& assets, RenderPalette& palette, Input& input,
                 RenderEnvironment& environment) {
            palette.asset = PaletteAsset;
            environment.ambient = {0.1F, 0.1F, 0.1F};
            environment.skyGradient[0] = {0.2F, 0.4F, 0.8F};
            environment.skyGradient[1] = {0.6F, 0.6F, 0.8F};
            input.bind(*assets.read(InputBindingsAsset));
            commands.spawn(assets.read(SceneAsset)->blueprint);
        });

    cubos.system("restart the game on input")
        .call([](Commands cmds, const Assets& assets, Score& score, const Input& input, AccumulatedTime& at,
                 Query<Entity> all) {
            if (input.justPressed("restart"))
            {
                resetGame(cmds, at, assets, score, all);
            }
        });

    cubos.system("detect player vs obstacle collisions")
        .without<Armor>()
        .call([](Commands cmds, const Assets& assets, Score& score, AccumulatedTime& at,
                 Query<Entity, const Player&, const CollidingWith&, const Obstacle&> collisions, Query<Entity> all) {
            for (auto [playerEntity, player, collidingWith, obstacle] : collisions)
            {
                resetGame(cmds, at, assets, score, all);
            }
        });

    cubos.run();
}
