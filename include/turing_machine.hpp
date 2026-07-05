#pragma once
#include<bits/stdc++.h>
#include "dynamic_tape.hpp"
#include "result.hpp"
#include "step_info.hpp"
#include "transition.hpp"
#include "transition_table.hpp"
class console_render;
using namespace std;
class turing_machine
{
    private:
string current_state;
string start_state;
set<string>acceptate_states;
set<string>rejected_states;
int maximum_steps=500;
int step_counter=0;
dynamic_tape tape;
transition_table table;


    public:
void set_start_state(const string & input);
void add_accepted_state(const string & input);
void add_rejected_state(const string & input);
void set_max_steps(int input);
void add_transition(const transition & input);
void initial_tape(const string & input);
bool step();
result run(console_render & render);
result run_head();
};