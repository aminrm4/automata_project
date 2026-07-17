#include "console_render.hpp"

#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <chrono>
#include <cstdlib>
#include <string>
#include <thread>

namespace
{

    std::string resultToString(result result)
    {
        switch (result)
        {
        case result::accepted:
            return "accepted";
        case result::rejected:
            return "rejected";
        case result::time_out:
            return "time_out";
        }
        return "unknown";
    }

    ftxui::Element buildTapeView(const step_info &info)
    {
        using namespace ftxui;

        if (info.tape_content.empty())
        {
            return text("_") | bgcolor(Color::Yellow) | bold | center | size(WIDTH, EQUAL, 3);
        }

        Elements cells;
        for (int i = 0; i < static_cast<int>(info.tape_content.size()); ++i)
        {
            const char symbol = info.tape_content[static_cast<std::size_t>(i)];
            Element cell = text(std::string(1, symbol)) | center | size(WIDTH, EQUAL, 3);
            if (i == info.head_position)
            {
                cell = cell | bgcolor(Color::Yellow) | bold;
            }
            else if (symbol == '_')
            {
                cell = cell | color(Color::GrayDark);
            }
            cells.push_back(cell);
        }
        return hbox(std::move(cells));
    }

    ftxui::Element buildView(const step_info &info, result final_result, bool show_final)
    {
        using namespace ftxui;

        Elements content;
        content.push_back(text("Turing Machine Simulator") | bold | center);
        content.push_back(separator());
        content.push_back(hbox({text("State: "), text(info.state) | color(Color::Cyan) | bold}));
        content.push_back(text("Tape:"));
        content.push_back(buildTapeView(info));
        content.push_back(hbox({text("Head Position: "), text(std::to_string(info.head_position))}));
        content.push_back(hbox({text("Step: "), text(std::to_string(info.step_number))}));

        if (show_final)
        {
            const Color resultColor = final_result == result::accepted ? Color::Green : Color::Red;
            content.push_back(separator());
            content.push_back(hbox({text("Final Result: "),
                                    text(resultToString(final_result)) | bold | color(resultColor)}));
            content.push_back(text("Press any key to exit...") | dim);
        }
        else
        {
            content.push_back(separator());
            content.push_back(text("[Space/N] Next step   [C] Continuous   [Q] Quit") | dim);
        }

        return vbox(std::move(content)) | border;
    }

} // namespace

console_render ::console_render() : console_render(std::getenv("AUTOMATA_BATCH") != nullptr) {}

console_render::console_render(bool batchMode)
    : screen(ftxui::ScreenInteractive::Fullscreen()),
      component(buildComponent()),
      loop(&screen, component)
{
    batch_mode = batchMode;
}

ftxui::Component console_render::buildComponent()
{
    using namespace ftxui;

    Component renderer = Renderer([this]
                                  { return buildView(currnet_info, final_result, has_final_result); });

    renderer |= CatchEvent([this](const Event &event)
                           {
        if (has_final_result) {
            advance_requested = true;
            return true;
        }

        if (event.is_character()) {
            const char key = event.character()[0];
            if (key == ' ' || key == 'n' || key == 'N') {
                advance_requested = true;
                return true;
            }
            if (key == 'c' || key == 'C') {
                mode = run_mode::Continuous;
                advance_requested = true;
                return true;
            }
            if (key == 'q' || key == 'Q') {
                quit_requested = true;
                mode = run_mode::Quit;
                advance_requested = true;
                return true;
            }
        }

        if (event == Event::Escape) {
            quit_requested = true;
            mode = run_mode::Quit;
            advance_requested = true;
            return true;
        }

        return false; });

    return renderer;
}

void console_render::render(const step_info &info)
{
    if (batch_mode)
    {
        return;
    }

    currnet_info = info;
    advance_requested = false;

    if (has_final_result)
    {
        while (!advance_requested && !loop.HasQuitted())
        {
            loop.RunOnceBlocking();
        }
        return;
    }

    if (mode == run_mode::Continuous)
    {
        screen.PostEvent(ftxui::Event::Custom);
        loop.RunOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(180));
        return;
    }

    while (!advance_requested && !loop.HasQuitted())
    {
        loop.RunOnceBlocking();
    }
}

void console_render::set_final_result(result result)
{
    final_result = result;
    has_final_result = true;
}