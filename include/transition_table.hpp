#pragma once

#include "transition.hpp"

#include <bits/stdc++.h>
using namespace std;

class transition_table
{
public:
    void add_transition(const transition &input);
    bool find_transition(const string &state, char symbol,transition &out_result) const;

private:
   map<pair<string, char>, transition> transitions;
};
