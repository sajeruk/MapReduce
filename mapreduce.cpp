#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <string>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <queue>
#include <sstream>

constexpr size_t CHUNK_SZ = 100000;
constexpr size_t MAX_CHUNK_SZ = 1000000;

namespace bp = ::boost::process;

struct Node {
    std::string key;
    int stream_id;
    Node(std::string ikey = "", int istream_id = 0) :
            key(ikey), stream_id(istream_id) {};
};

struct Comporator {
  bool operator()(const Node& lhs, const Node& rhs) const {
    return lhs.key > rhs.key;
  }
};

bp::child start_child(const std::string& exec, std::vector<std::string>& args) {
    bp::context ctx;
    ctx.stdin_behavior = bp::capture_stream();
    ctx.stdout_behavior = bp::capture_stream();

    return bp::launch(exec, args, ctx);
}

bp::child map_(const std::string& script_name,
                std::vector<std::string>& args, const std::string& str){
    bp::child map_proc = start_child(script_name, args);

    bp::postream &os = map_proc.get_stdin();
    os << str << std::endl;
    os.close();

    return map_proc;
}

bp::child reduce(const std::string& script_name, std::vector<std::string>& args,
                const std::string& key, std::vector<std::string>& values) {
    bp::child reduce_proc = start_child(script_name, args);

    bp::postream &os = reduce_proc.get_stdin();
    for (auto val : values) {
        os << key << "\t" << val << std::endl;
    }
    os.close();

    return reduce_proc;
}

void mergeChunks(const std::string& output_file, const int& chunk_cnt) {
    std::priority_queue<Node, std::vector<Node>, Comporator> heap;

    std::string key;
    std::vector<std::ifstream> chunk(chunk_cnt + 1);
    for (int id = 0; id <= chunk_cnt; id++) {
        std::string chunk_name = "output" + std::to_string(id) + ".txt";
        chunk[id] = std::ifstream(chunk_name);
        chunk[id] >> key;
        heap.push(Node(key, id));
    }

    std::ofstream fout(output_file);

    Node cur_min;
    std::string value;
    std::string next_key;
    while (!heap.empty()) {
        cur_min = heap.top();
        heap.pop();

        getline(chunk[cur_min.stream_id], value);
        fout << cur_min.key << value << '\n';

        if (chunk[cur_min.stream_id] >> next_key) {
            if (next_key.size() > 0) {
                heap.push(Node(next_key, cur_min.stream_id));
            }
        }
    }

    fout.close();
}

std::vector<std::string> sortChunk(std::vector<char> &buffer) {
    std::vector<std::string> data;
    std::stringstream ss(&buffer[0]);
    std::string k_v;

    while (std::getline(ss, k_v, '\n')) {
        data.push_back(k_v);
    }

    std::sort(data.begin(), data.end());

    return data;
}

void recordToChunk(std::vector<std::string> &data, const std::string& output) {
    std::ofstream fout(output, std::ifstream::binary);
    for(auto i : data) {
        fout << i << std::endl;
    }
    fout.close();
}

bool makeChunk(std::ifstream &fin, const int& chunk_id) {
    std::vector<char> buffer(MAX_CHUNK_SZ, 0);

    auto elem_cnt = fin.readsome(&buffer[0], CHUNK_SZ);
    if (!elem_cnt || (buffer[0] == '\n')) {
        return false;
    }

    auto last_ind = elem_cnt - 1;
    while (buffer[last_ind] != '\n') {
        fin.read(&buffer[++last_ind], 1);
    }

    std::vector<std::string> sorted_data = sortChunk(buffer);

    std::string output_name = "output" + std::to_string(chunk_id) + ".txt";
    recordToChunk(sorted_data, output_name);
}

int splitIntoChunks(const std::string& input_file) {
    std::ifstream fin(input_file, std::ifstream::binary);

    int chuck_id = 0;
    while (makeChunk(fin, chuck_id)) {
        chuck_id++;
    }

    fin.close();

    return chuck_id - 1;
}

void externalSort(const std::string& input_file,
                    const std::string& output_file) {
    int chunk_cnt = splitIntoChunks(input_file);
    mergeChunks(output_file, chunk_cnt);
}

std::string keyGroup(std::ifstream& fin,
            std::pair<std::string, std::vector<std::string>>& group) {
    std::string delimiter = "\t";
    std::string value;
    std::string cur_key = group.first;

    do {
        if (cur_key != group.first) {
          break;
        }

        getline(fin, value);
        group.second.push_back(value.substr(
                    value.find(delimiter) + 1, value.size()));
    } while (fin >> cur_key);

    return cur_key;
}

void out(std::ofstream& fout, bp::pistream& child_os) {
    std::string line;
    while (std::getline(child_os, line)) {
        fout << line << std::endl;
    }
}

int main(int argc, char* argv[])
{
    enum func_args{mode = 1, scr_path = 2,
            default_num_of_args = 5, id_add_args = 3};
    std::vector<std::string> str_args(argv, argv + argc);

    std::vector<std::string> script_arg = {"-o"};
    if (argc > default_num_of_args) {
        for (int i = id_add_args; i < argc - 2; i++) {
            script_arg.push_back(str_args[i]);
        }
    }

    std::ofstream fout(str_args[argc - 1]);

    if (str_args[mode] == "map") {

        std::ifstream fin(str_args[argc - 2]);
        for (std::string str; std::getline(fin, str);) {
            bp::child map_proc = map_(str_args[scr_path], script_arg, str);
            out(fout, map_proc.get_stdout());
        }
         fin.close();

    } else if (str_args[mode] == "reduce") {

        externalSort(str_args[argc - 2], "_output.txt");

        std::ifstream fin("_output.txt");

        std::string prev_key;
        fin >> prev_key;
        while (!fin.eof()) {
            std::pair<std::string, std::vector<std::string>> k_v;

            k_v.first = prev_key;
            prev_key = keyGroup(fin, k_v);

            bp::child reduce_proc = reduce(str_args[scr_path], script_arg,
                    k_v.first, k_v.second);
            out(fout, reduce_proc.get_stdout());
        }
        fin.close();

    }

    fout.close();

    return 0;
}
