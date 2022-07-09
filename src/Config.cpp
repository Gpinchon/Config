#include <Config.hpp>

#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdio.h> // for sscanf, fgets, fopen
#include <vector>

namespace Config {
void File::Parse(const std::filesystem::path path)
{
    std::ifstream configFile(path);
    for (std::string line; std::getline(configFile, line);) {
        std::istringstream iss(line);
        std::vector<std::string> words(
            std::istream_iterator<std::string> { iss },
            std::istream_iterator<std::string>());
        if (words.size() > 2 && words.at(1) == "=") {
            switch (words.size() - 2) {
            case 1:
                try {
                    auto value{ std::stof(words.at(2)) };
                    _configMap[words.at(0)] = value;
                }
                catch (std::exception&) {
                    _configMap[words.at(0)] = words.at(2);
                }
                break;
            }
        }
    }
}

void File::Save(const std::filesystem::path path)
{
    std::ofstream configFile;
    configFile.open(path);
    for (const auto& v : _configMap) {
        configFile << v.first << " = ";
        switch (v.second.index()) {
        case 0:
            configFile << std::get<0>(v.second);
            break;
        case 1:
            configFile << std::get<1>(v.second);
            break;
        }
        configFile << '\n';
    }
}

File& Config::Global()
{
    static Config::File instance;
    return instance;
}

}