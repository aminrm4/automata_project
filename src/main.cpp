#include "console_render.hpp"
#include "machine_loader.hpp"
#include "result.hpp"
#include "turing_machine.hpp"

#include <bits/stdc++.h>
using namespace std;
namespace
{

    std::string result_to_strig(result result)
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

    void print_usage(char *program)
    {
        cerr << program << ".json run for test" << endl;
    }

}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    bool batch_mode = false;
    if (argc == 3)
    {
        string flag = argv[2];
        if (flag == "--batch")
        {
            batch_mode = true;
        }
        else
        {
            print_usage(argv[0]);
            return 1;
        }
    }

    try
    {
        turing_machine machine;
        machine_loader loader;
        loader.load_from_json(argv[1], machine);

        result result;

        if (batch_mode)
        {
            result = machine.run_head();
        }
        else
        {
            console_render renderer(false);
            result = machine.run(renderer);
        }

        cout << result_to_strig(result) << '\n';
        return 0;
    }
    catch (exception &ex)
    {
        cerr << "error: " << ex.what() << '\n';
        return 1;
    }
}
