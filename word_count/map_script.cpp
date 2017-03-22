#include <iostream>
#include <string>
#include <sstream>
#include <map>

int main()
{
    std::string delimiter = "\t";
    std::map<std::string, int> word_count;

    for (std::string str; std::getline(std::cin, str); ) {

        std::string key = str.substr(0, str.find(delimiter));
        std::string text = str.substr(str.find(delimiter) + 1, str.size()) + ' ';

        std::istringstream iss(text);

        for (std::string s; iss >> s; ) {
            word_count[s]++;
        }

    }

    for (auto i : word_count) {
        std::cout << i.first << "\t" << i.second << std::endl;
    }

    return 0;
}
