#include <algorithm>
#include <iostream>

char* get_opt(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

bool opt_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void print_help(char* program) {
    std::cout << program << "[options]" << std::endl <<
         "Options:" << std::endl <<
         "-h    Print this help" << std::endl <<
         "-g    Enable undirected graph generation mode" <<
         "-f    Path to graph binary file for reading, or writing during graph generation mode" <<
         "-n    Number of vertices for graph generation mode" <<
         "-p    Probablity for edge existence for graph generation mode" <<
         "-s    Seed rng used during graph generation mode" << std::endl;
}