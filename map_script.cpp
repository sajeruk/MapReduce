#include <iostream>
##include <iostream>
#include <string>
#include <sstream>
#include <map>

bool isNumber(std::string input) {
    return ((input[0] >= '0' && input[0] <= '9'));
}

int main()
{
    std::string delimiter = "\t";
    std::map<std::string, std::string> word_url;

    for (std::string str; std::getline(std::cin, str); ) {

        std::string url = str.substr(0, str.find(delimiter));
        std::string text = str.substr(str.find(delimiter) + 1, str.size()) + ' ';

        std::istringstream iss(text);
        for (std::string word; iss >> word; ) {
            if (word.size() < 3 || isNumber(word)) {
                continue;
            }
            word_url[word] = url;
        }

    }

    for (auto i : word_url) {
        std::cout << i.first << "\t" << i.second << std::endl;
    }

    return 0;
}
