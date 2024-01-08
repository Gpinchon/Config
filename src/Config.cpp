#include <Config.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>

namespace Config
{
    void File::Parse(const std::filesystem::path &path)
    {
        std::ifstream configFile(path);
        for (std::string line; std::getline(configFile, line);)
        {
            std::istringstream iss(line);
            std::vector<std::string> words(
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>());
            if (words.size() > 2 && words.at(1) == "=")
            {
                for (auto i = 0u; i < words.size(); ++i)
                {
                    try
                    {
                        auto value{std::stof(words.at(i))};
                        _configMap[words.at(0)].emplace_back(value);
                    }
                    catch (std::exception &)
                    {
                        _configMap[words.at(0)].emplace_back(i);
                    }
                }
            }
        }
    }

    void File::Save(const std::filesystem::path &path)
    {
        std::ofstream configFile;
        configFile.open(path);
        for (const auto &v : _configMap)
        {
            configFile << v.first << " =";
            for (const auto &value : v.second)
            {
                switch (value.index())
                {
                case 0:
                    configFile << ' ' << std::get<0>(value);
                    break;
                case 1:
                    configFile << ' ' << std::get<1>(value);
                    break;
                }
            }
            configFile << '\n';
        }
    }

    File &Global()
    {
        static Config::File instance;
        return instance;
    }
}