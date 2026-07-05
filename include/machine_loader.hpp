#pragma once

#include "turing_machine.hpp"

#include <bits/stdc++.h>
using namespace std;

class machine_loader
{
public:
    void load_from_json(const string &path, turing_machine &machine);
};
