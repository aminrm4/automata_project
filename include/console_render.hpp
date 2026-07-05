#pragma once

#include "result.hpp"
#include "step_info.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>

class console_render
{
public:
    enum class run_mode
    {
        Step,
        Continuous,
        Quit
    };

    console_render();
    explicit console_render(bool batch_mode);

    void render(const step_info &info);

    void set_final_result(result result);

    bool should_quit() const { return quit_requested; }

private:
   
    ftxui::Component buildComponent();

    run_mode mode = run_mode::Step;
    bool quit_requested = false;
    bool advance_requested = false;
    bool batch_mode = false;

    result final_result = result::rejected;
    bool has_final_result = false;

    step_info currnet_info;

    ftxui::ScreenInteractive screen;
    ftxui::Component component;
    ftxui::Loop loop;
};
