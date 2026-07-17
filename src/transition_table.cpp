#include "transition_table.hpp"

void transition_table::add_transition(const transition& trn){
    transitions[{trn.current_state, trn.read_symbol}] = trn;
}

bool transition_table::find_transition
(
    const string& state, 
    char symbol,
    transition& out_result

) const {

    const auto it = transitions.find({state, symbol});
    if(it == transitions.end()){
        return false;
    }

    out_result = it->second;
    return true;
}
