#pragma once

#include <bits/stdc++.h>
using namespace std;
struct transition
{
    string current_state;
    char read_symbol;
    string next_state;
    char write_symbol;
    char direction;
};
