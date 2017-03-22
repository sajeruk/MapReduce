#include <iostream>
#include <string>

int main()
{
    int word_count = 0;
    std::string delimiter = "\t";
    std::string key = "";

    for (std::string str; std::getline(std::cin, str); ) {

        key = str.substr(0, str.find(delimiter));
        std::string value = str.substr(str.find(delimiter) + 1, str.size());

        word_count += std::stoi(value);

    }

    std::cout << key << "\t" << word_count << std::endl;

    return 0;
}
