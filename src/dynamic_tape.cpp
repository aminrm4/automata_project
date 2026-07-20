#include "dynamic_tape.hpp"

#include <bits/stdc++.h>

void dynamic_tape::clear()
{
    tape.clear();
    head = 0;
    print_start = 0;
}

void dynamic_tape::load_initial(const std::string &initial)
{
    clear();
    tape.assign(initial.begin(), initial.end());
    update_bounds();
}

char dynamic_tape::read() const
{
    if (head < 0 || head >= static_cast<int>(tape.size()))
    {
        return '_';
    }
    return tape[head];
}

void dynamic_tape::write(char c)
{
    ensure_exists(head);
    tape[head] = c;
    update_bounds();
}

void dynamic_tape::move_left()
{
    head--;
    if (head < 0)
    {
        expand_left();
    }
}

void dynamic_tape::move_right()
{
    head++;
    if (head >= static_cast<int>(tape.size()))
    {
        expand_right();
    }
}

void dynamic_tape::expand_left()
{
    tape.insert(tape.begin(), numbers_of_expand, '_');
    head += numbers_of_expand;
    print_start += numbers_of_expand;
}

void dynamic_tape::expand_right()
{
    tape.insert(tape.end(), numbers_of_expand, '_');
    update_bounds();
}

std::string dynamic_tape::print() const
{
    if (tape.empty())
    {
        return "";
    }

    int end = static_cast<int>(tape.size()) - 1;
    while (end > print_start && tape[end] == '_')
    {
        end--;
    }

    const int start = print_start;
    if (start > end)
    {
        return "_";
    }

    return string(tape.begin() + start, tape.begin() + end + 1);
}

int dynamic_tape::get_head_position_bounds() const { return head - print_start; }

void dynamic_tape::ensure_exists(int index)
{
    if (index < 0)
    {
        const int deficit = -index;
        const int blocks = (deficit + numbers_of_expand - 1) / numbers_of_expand;
        for (int i = 0; i < blocks; ++i)
        {
            expand_left();
        }
    }

    if (index >= static_cast<int>(tape.size()))
    {
        const int deficit = index - static_cast<int>(tape.size()) + 1;
        const int blocks = (deficit + numbers_of_expand - 1) / numbers_of_expand;
        for (int i = 0; i < blocks; ++i)
        {
            expand_right();
        }
    }
}

void dynamic_tape::update_bounds()
{
    if (tape.empty())
    {
        print_start = 0;
        return;
    }

    int leftmost = head;
    int rightmost = head;

    for (int i = 0; i < static_cast<int>(tape.size()); i++)
    {
        if (tape[i] != '_')
        {
            leftmost =min(leftmost, i);
            rightmost = max(rightmost, i);
        }
    }

    print_start =min(print_start, leftmost);
    rightmost;
}
