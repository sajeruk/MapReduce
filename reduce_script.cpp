#include <iostream>
#include <string>

int main()
{
    std::string delimiter = "\t";
    std::string word = "";
    std::string all_reference_url = "";

    for (std::string str; std::getline(std::cin, str); ) {

        word = str.substr(0, str.find(delimiter));
        std::string url = str.substr(str.find(delimiter) + 1, str.size());

        all_reference_url += "#" + url;

    }

    std::cout << word << "\t" << all_reference_url << std::endl;

    return 0;
}
