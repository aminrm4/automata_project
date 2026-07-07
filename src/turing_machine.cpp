#include "turing_machine.hpp"
#include "console_render.hpp"

void turing_machine::set_start_state(const string& input) { start_state = input; }

void turing_machine::add_accepted_state(const string& input) { acceptate_states.insert(input); }

void turing_machine::add_rejected_state(const string& input) { rejected_states.insert(input); }

void turing_machine::set_max_steps(int input) { maximum_steps = input; }

void turing_machine::add_transition(const Transition& input) { table.add_transition(input); }

void turing_machine::initial_tape(const string& input){

    tape.load_initial(input);
    step_counter = 0;
    current_state = start_state;
}

bool turing_machine::step(){

    transition trn;
    const char symbol = tape.read();
    
    if(!table.find_transition(current_state, symbol, trn)){
        return false;
    }


    tape.write(trn.write_symbol);
    if(trn.direction == 'L'){
        tape.move_left();
    } 
    else{
        tape.move_right();
    }

    current_state = trn.next_state;
    ++step_counter;
    return true;
}

step_info turing_machine::build_step_info() const{

    step_info info;

    info.state = current_state;
    info.tape_content = tape.print();
    info.head_position = tape.get_head_position_bounds();
    info.step_number = step_counter;

    return info;
}
