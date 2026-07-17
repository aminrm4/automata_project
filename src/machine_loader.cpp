#include "machine_loader.hpp"
#include <nlohmann/json.hpp>

namespace{

    void require_field(const nlohmann::json& json, const char* key){

        if(!json.contains(key)){
            throw runtime_error(string("Missing required field: ") + key);
        }
    }

    char parse_symbol(const nlohmann::json& value, const char* field_name){

        if(!value.is_string() || value.get<string>().empty()){
            throw runtime_error(string("Invalid symbol for field: ") + field_name);
        }
        return value.get<string>()[0];
    }

    char parse_direction(const nlohmann::json& value){

        const char direction = parse_symbol(value, "dir");
        if(direction != 'L' && direction != 'R'){
            throw runtime_error("Transition direction must be 'L' or 'R'");
        }
        return direction;
    }
}  // namespace

void machine_loader::load_from_json
(
    const string& path, 
    turing_machine& machine
){


    ifstream input(path);
    if(!input.is_open()){
        throw runtime_error("Could not open JSON file: " + path);
    }

    nlohmann::json json;
    input >> json;

    require_field(json, "accept_states");
    require_field(json, "reject_states");
    require_field(json, "start_state");
    require_field(json, "max_steps");
    require_field(json, "transitions");
    require_field(json, "initial_tape");

    for(const auto& state: json.at("accept_states")){
        machine.add_accepted_state(state.get<string>());
    }

    for(const auto& state : json.at("reject_states")){
        machine.add_rejected_state(state.get<string>());
    }

    machine.set_start_state(json.at("start_state").get<string>());
    machine.set_max_steps(json.at("max_steps").get<int>());

    for(const auto& item: json.at("transitions")){

        require_field(item, "from");
        require_field(item, "read");
        require_field(item, "to");
        require_field(item, "write");
        require_field(item, "dir");

        transition trn;
        trn.current_state = item.at("from").get<string>();
        trn.read_symbol = parse_symbol(item.at("read"), "read");
        trn.next_state = item.at("to").get<string>();
        trn.write_symbol = parse_symbol(item.at("write"), "write");
        trn.direction = parse_direction(item.at("dir"));
        machine.add_transition(trn);
    }

    machine.initial_tape(json.at("initial_tape").get<string>());
}
