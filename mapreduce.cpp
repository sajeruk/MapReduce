#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <string>
#include <cstdio>
#include <vector>
#include <unordered_map>

namespace bp = ::boost::process;

bp::child start_child(char* exec1, std::vector<std::string> &args) {
    std::string exec = exec1;

    bp::context ctx;
    ctx.stdin_behavior = bp::capture_stream();
    ctx.stdout_behavior = bp::capture_stream();

    return bp::launch(exec, args, ctx);
}

bp::child map_(char* script_name, std::vector<std::string> &args,
                    std::string str){
    bp::child map_proc = start_child(script_name, args);

    bp::postream &os = map_proc.get_stdin();
    os << str << std::endl;
    os.close();

    return map_proc;
}

bp::child reduce(char* script_name, std::vector<std::string> &args,
            const std::string &key, std::vector<std::string> &values) {
    bp::child reduce_proc = start_child(script_name, args);

    bp::postream &os = reduce_proc.get_stdin();
    for (auto val : values) {
        os << key << "\t" << val << std::endl;
    }
    os.close();

    return reduce_proc;
}

void group(std::ifstream &fin,
             std::unordered_map<std::string, std::vector<std::string>> &data) {
    std::string delimiter = "\t";
    for (std::string str; std::getline(fin, str);) {
        std::string key = str.substr(0, str.find(delimiter));
        std::string value = str.substr(str.find(delimiter) + 1, str.size());
        data[key].push_back(value);
    }
}

void out(std::ofstream &fout, bp::pistream &child_os) {
    std::string line;
    while (std::getline(child_os, line)) {
        fout << line << std::endl;
    }
}

int main(int argc, char* argv[])
{
    enum func_args{mode = 1, scr_path = 2,
            default_num_of_args = 5, id_add_args = 3};
    std::ifstream fin(argv[argc - 2]);
    std::ofstream fout(argv[argc - 1]);

    std::vector<std::string> script_arg = {"-o"};
    if (argc > default_num_of_args) {
        for (int i = id_add_args; i < argc - 2; i++) {
            script_arg.push_back(argv[i]);
        }
    }

    if (!strcmp(argv[mode], "map")) {

        for (std::string str; std::getline(fin, str);) {
            bp::child map_proc = map_(argv[scr_path], script_arg, str);
            out(fout, map_proc.get_stdout());
        }

    } else if (!strcmp(argv[mode], "reduce")) {
        std::unordered_map<std::string, std::vector<std::string>> input;

        group(fin, input);

        for (auto k_v : input) {
            bp::child reduce_proc = reduce(argv[scr_path], script_arg,
                k_v.first, k_v.second);
            out(fout, reduce_proc.get_stdout());
        }
    }

    fin.close();
    fout.close();

    return 0;
}
