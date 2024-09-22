#include "gui.hpp"

#include <imgui.h>
#include <cubos/core/ecs/reflection.hpp>
#include <cubos/engine/imgui/data_inspector.hpp>
#include <cubos/engine/imgui/plugin.hpp>
#include <cubos/engine/render/target/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/window/plugin.hpp>

#include "score.hpp"
#include "player.hpp"
#include "jetpack.hpp"
#include "armor.hpp"
#include "accumulated_time.hpp"

using namespace cubos::engine;

void guiPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(settingsPlugin);
    cubos.depends(windowPlugin);
    cubos.depends(renderTargetPlugin);
    cubos.depends(imguiPlugin);

    cubos.depends(scorePlugin);
    cubos.depends(playerPlugin);
    cubos.depends(armorPlugin);
    cubos.depends(jetpackPlugin);
    cubos.depends(accumulatedTimePlugin);

    cubos.system("ImGUI score")
        .tagged(imguiTag)
        .call([](const Score& score, const AccumulatedTime& at, Query<const Player&, const Jetpack&> jetpackQuery) {
            ImGui::Begin("Score Window!");
            ImGui::Text("Score: %d", score.score);
            ImGui::Text("Time: %f", at.time);
            for (auto [player, jetpack] : jetpackQuery)
            {
                ImGui::Text("Jetpack remaining time: %f", jetpack.duration);
            }
            ImGui::End();
        });
}
