#include "score.hpp"

#include <imgui.h>
#include <cubos/core/ecs/reflection.hpp>
#include <cubos/engine/imgui/data_inspector.hpp>
#include <cubos/engine/imgui/plugin.hpp>
#include <cubos/engine/render/target/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/window/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Score)
{
    return cubos::core::ecs::TypeBuilder<Score>("Score")
        .withField("scene", &Score::score)
        .build(); 
}

void scorePlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(settingsPlugin);
    cubos.depends(windowPlugin);
    cubos.depends(renderTargetPlugin);
    cubos.depends(imguiPlugin);

    cubos.resource<Score>();

    cubos.system("ImGUI score")
        .tagged(imguiTag)
        .call([]() {
            ImGui::Begin("Score Window!");
            // ImGui::Text("Hello world!");
            ImGui::End();

            ImGui::ShowDemoWindow();
        });
}
