#pragma once

#include <filesystem>
#include <map>
#include <variant>

/**
* This is the common configuration between APIs
*/
namespace Config {
class File {
public:
    /**
     * @brief Loads the Config from the specified file, invalid values will be ignored (set to default value) but setting keys will still be registered
     * @param path the absolute path to the config file
     */
    void Parse(const std::filesystem::path path);
    /**
     * @brief Saves this Config to the specified file
     * @param path the absolute path to the file this Config is to be saved to
     */
    void Save(const std::filesystem::path path);
    /**
     * @brief Tries to get the specified setting, set it to the default value if not found
     * @tparam T the type that we expect
     * @param name the name of the setting
     * @param defaultValue the default value the setting is to be set to if it is non-existent
     * @return the value of the setting specified by name
     */
    template <typename T>
    T Get(const std::string& name, const T defaultValue);
    /**
     * @brief Sets the specified setting to the specified value
     * @tparam T the type of this setting
     * @param name the name of the setting to set
     * @param value the value the setting is to be set to
     * @return the new value of the setting specified by name
     */
    template <typename T>
    T Set(const std::string& name, const T value);

private:
    std::map<std::string, std::variant<float, std::string>> _configMap;
};

File& Global();

template <>
inline int File::Get<int>(const std::string& name, const int defaultValue)
{
    return Get<float>(name, defaultValue);
}

template <>
inline unsigned File::Get<unsigned>(const std::string& name, const unsigned defaultValue)
{
    return Get<float>(name, defaultValue);
}

template <typename T>
inline T File::Get(const std::string& name, const T defaultValue)
{
    auto it = _configMap.find(name);
    if (it != _configMap.end())
        return std::get<T>((*it).second);
    else
        return File::Set(name, defaultValue);
}

template <typename T>
inline T File::Set(const std::string& name, const T value)
{
    return std::get<T>(_configMap[name] = value);
}
}