#include <iostream>
#include <string>

int main()
{
    std::string delimiter = "\t";
    for (std::string str; std::getline(std::cin, str); ) {
      std::string key = str.substr(0, str.find(delimiter));
      std::string value = str.substr(str.find(delimiter) + 1, str.size());
      std::cout << key << '\t' << value << std::endl;
    }
    return 0;
}
