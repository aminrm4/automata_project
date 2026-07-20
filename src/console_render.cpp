#include "console_render.hpp"

#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <bits/stat.h>
#include <chrono>
#include <cstdlib>
#include <string>
#include <thread>
using namespace std;
namespace
{

     size_t maximum_store_points = 60;

    string result_to_string(result result)
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

    ftxui::Element build_tape_view(const step_info &info)
    {
        using namespace ftxui;

        if (info.tape_content.empty())
        {
            Element marker = text("▼") | color(Color::Yellow) | bold | center | size(WIDTH, EQUAL, 5);
            Element cell = text("_") | bgcolor(Color::Yellow) | color(Color::Black) | bold | center | size(WIDTH, EQUAL, 5) | borderStyled(ROUNDED, Color::YellowLight);
            return vbox({marker, cell});
        }

        Elements markers;
        Elements cells;
        for (int i = 0; i < static_cast<int>(info.tape_content.size()); ++i)
        {
            const char symbol = info.tape_content[static_cast<size_t>(i)];
            Element cell = text(string(1, symbol)) | center | size(WIDTH, EQUAL, 5) | size(HEIGHT, EQUAL, 3);

            if (i == info.head_position)
            {
                markers.push_back(text("▼") | color(Color::Yellow) | bold | center | size(WIDTH, EQUAL, 5));
                cell = cell | bgcolor(Color::Yellow) | color(Color::Black) | bold | borderStyled(ROUNDED, Color::YellowLight);
            }
            else
            {
                markers.push_back(text(" ") | center | size(WIDTH, EQUAL, 5));
                if (symbol == '_')
                {
                    cell = cell | color(Color::GrayDark) | borderStyled(ROUNDED, Color::GrayDark);
                }
                else
                {
                    cell = cell | color(Color::CyanLight) | bold | borderStyled(ROUNDED, Color::Cyan);
                }
            }
            cells.push_back(cell);
        }
        return vbox({hbox(move(markers)), hbox(move(cells))});
    }

    ftxui::Element build_head_history_graph(const vector<int> &head_history)
    {
        using namespace ftxui;

        auto graphFn = [history = head_history](int width, int height) -> vector<int>
        {
            vector<int> result(static_cast<size_t>(max(width, 0)), 0);
            if (history.empty() || width <= 0 || height <= 0)
            {
                return result;
            }

            const int minVal = *min_element(history.begin(), history.end());
            const int maxVal = *max_element(history.begin(), history.end());
            const int range = max(1, maxVal - minVal);
            const int count = static_cast<int>(history.size());

            for (int x = 0; x < width; ++x)
            {
                const int idx = count > 1 ? min((x * (count - 1)) / max(1, width - 1), count - 1) : 0;
                const int value = history[static_cast<size_t>(idx)];
                result[static_cast<size_t>(x)] = ((value - minVal) * (height - 1)) / range;
            }
            return result;
        };

        return graph(move(graphFn));
    }

    ftxui::Element build_view(const step_info &info, result final_result, bool show_final, const vector<int> &head_history)
    {
        using namespace ftxui;

        Elements content;

        content.push_back(text("╔═══════════════════════════╗") | color(Color::CyanLight));
        content.push_back(hbox({text("║ ") | color(Color::CyanLight),
                                text("TURING MACHINE SIMULATOR") | bold | color(Color::White),
                                text(" ║") | color(Color::CyanLight)}) |
                          center);
        content.push_back(text("╚═══════════════════════════╝") | color(Color::CyanLight));
        content.push_back(text(""));

        content.push_back(hbox({text(" ⚙ STATE ") | bold | bgcolor(Color::Blue) | color(Color::White),
                                text(" "),
                                text(" " + info.state + " ") | color(Color::Cyan) | bold | borderStyled(ROUNDED, Color::Cyan)}) |
                          center);

        content.push_back(text(""));
        content.push_back(text("▬▬▬ TAPE ▬▬▬") | dim | color(Color::GrayDark) | center);
        content.push_back(build_tape_view(info) | center);
        content.push_back(text(""));

        content.push_back(hbox({text(" HEAD ") | bold | bgcolor(Color::Magenta) | color(Color::White),
                                text(" " + to_string(info.head_position) + " ") | bold | color(Color::White) | borderStyled(ROUNDED, Color::GrayDark),
                                text("   "),
                                text(" STEP ") | bold | bgcolor(Color::Green) | color(Color::Black),
                                text(" " + to_string(info.step_number) + " ") | bold | color(Color::White) | borderStyled(ROUNDED, Color::GrayDark)}) |
                          center);

        content.push_back(text(""));
        {
            const int tapeLength = static_cast<int>(info.tape_content.size());
            const float positionRatio = tapeLength > 1
                                            ? static_cast<float>(info.head_position) / static_cast<float>(tapeLength - 1)
                                            : 0.0f;
            content.push_back(hbox({text(" ⇤ ") | dim,
                                    gauge(positionRatio) | color(Color::Yellow) | bgcolor(Color::GrayDark) | flex,
                                    text(" ⇥ ") | dim}));
            content.push_back(hbox({text(to_string(static_cast<int>(positionRatio * 100)) + "% of tape traversed") | dim | color(Color::GrayDark)}) | center);
        }

        if (head_history.size() > 1)
        {
            content.push_back(text(""));
            content.push_back(text("▬▬▬ HEAD MOVEMENT ▬▬▬") | dim | color(Color::GrayDark) | center);
            content.push_back(build_head_history_graph(head_history) | color(Color::Green) | size(HEIGHT, EQUAL, 6) | flex);
        }

        if (show_final)
        {
            const Color resultColor = final_result == result::accepted ? Color::Green : Color::Red;
            content.push_back(text(""));
            content.push_back(separatorHeavy() | color(resultColor));
            content.push_back(hbox({text(" ★ RESULT ★ ") | bold,
                                    text(" " + result_to_string(final_result) + " ") | bold | bgcolor(resultColor) | color(Color::Black)}) |
                              center);
            content.push_back(text(""));
            content.push_back(text("· press any key to exit ·") | dim | center);
        }
        else
        {
            content.push_back(text(""));
            content.push_back(separatorLight() | color(Color::GrayDark));
            content.push_back(hbox({
                                  text(" SPACE") | bold | bgcolor(Color::Yellow) | color(Color::Black),
                                  text("/") | dim,
                                  text("N") | bold | bgcolor(Color::Yellow) | color(Color::Black),
                                  text(" next   ") | dim,
                                  text("C") | bold | bgcolor(Color::Cyan) | color(Color::Black),
                                  text(" continuous   ") | dim,
                                  text("Q") | bold | bgcolor(Color::Red) | color(Color::White),
                                  text(" quit ") | dim,
                              }) |
                              center);
        }

        return vbox(move(content)) | borderStyled(ROUNDED, Color::CyanLight) | color(Color::White) | size(WIDTH, GREATER_THAN, 46);
    }

}

console_render ::console_render() : console_render(getenv("AUTOMATA_BATCH") != nullptr) {}

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
                                  { return build_view(currnet_info, final_result, has_final_result, head_history); });

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

    head_history.push_back(info.head_position);
    if (head_history.size() > maximum_store_points)
    {
        head_history.erase(head_history.begin());
    }

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
        this_thread::sleep_for(chrono::milliseconds(180));
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