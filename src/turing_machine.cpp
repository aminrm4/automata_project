#include "turing_machine.hpp"
#include "console_render.hpp"

void turing_machine::set_start_state(const string& input) { start_state = input; }

void turing_machine::add_accepted_state(const string& input) { acceptate_states.insert(input); }

void turing_machine::add_rejected_state(const string& input) { rejected_states.insert(input); }

void turing_machine::set_max_steps(int input) { maximum_steps = input; }

void turing_machine::add_transition(const Transition& input) { table.addTransition(input); }

void turing_machine::initial_tape(const string& input){

    tape.load_initial(input);
    step_counter = 0;
    current_state = start_state;
}
