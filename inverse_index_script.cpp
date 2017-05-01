#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>

namespace bp = ::boost::process;

enum func_args{map_reduce = 1, from_url_to_text = 2,
                    map_script = 3, reduce_script = 4, default_num_of_args = 6};

bp::child start_child(const std::string& exec, std::vector<std::string>& args) {
    bp::context ctx;
    ctx.stdin_behavior = bp::capture_stream();
    ctx.stdout_behavior = bp::capture_stream();

    return bp::launch(exec, args, ctx);
}

void makeArgs(std::vector<std::string>& args, std::string mode,
        const std::string &script, const std::string &input,
        const std::string &output) {
    args[1] = mode;
    args[2] = script;
    args[3] = input;
    args[4] = output;
}

std::string getDataFromUrl(std::vector<std::string>& args,
                                    std::string map_script, std::string input) {
    std::string output = "data_from_wikipage.txt";
    makeArgs(args, "map", map_script, input, output);
    bp::child proc = start_child(args[0], args);
    proc.wait();
    return output;
}

std::string getAllWordsFromPages(std::vector<std::string>& args,
                                    std::string map_script, std::string input) {
    std::string output = "all_words.txt";
    makeArgs(args, "map", map_script, input, output);
    bp::child proc = start_child(args[0], args);
    proc.wait();
    return output;
}

void getInverseIndices(std::vector<std::string>& args,
                std::string reduce_script, std::string input, std::string output) {
    makeArgs(args, "reduce", reduce_script, input, output);
    bp::child proc = start_child(args[0], args);
    proc.wait();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    std::vector<std::string> script_arg(5);
    script_arg[0] = args[map_reduce];

    std::string wikipage_output = getDataFromUrl(script_arg,
        args[from_url_to_text], args[argc - 2]);

    std::string all_words = getAllWordsFromPages(script_arg,
                   args[func_args(map_script)], wikipage_output);

    getInverseIndices(script_arg, args[reduce_script],
                    all_words, args[argc - 1]);

    return 0;
}
