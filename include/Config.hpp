#pragma once

#include <filesystem>
#include <map>
#include <vector>
#include <variant>
#include <algorithm>

/**
 * This is the common configuration between APIs
 */
namespace Config
{
    class File
    {
    public:
        /**
         * @brief Loads the Config from the specified file, invalid values will be ignored (set to default value) but setting keys will still be registered
         * @param path the absolute path to the config file
         */
        void Parse(const std::filesystem::path &path);
        /**
         * @brief Saves this Config to the specified file
         * @param path the absolute path to the file this Config is to be saved to
         */
        void Save(const std::filesystem::path &path);

        /**
         * @brief Tries to get the specified setting, set it to the default value if not found
         * @param name the name of the setting
         * @param defaultValue the default value the setting is to be set to if it is non-existent
         * @param index the index to query, default is zero
         * @return the value of the setting specified by name
         */
        std::string Get(const std::string &name, const std::string &defaultValue, const size_t index = 0);
        /**
         * @brief Tries to get the specified setting, set it to the default value if not found
         * @param name the name of the setting
         * @param defaultValue the default value the setting is to be set to if it is non-existent
         * @param index the index to query, default is zero
         * @return the value of the setting specified by name
         */
        double Get(const std::string &name, const double &defaultValue, const size_t index = 0);

        /**
         * @brief Sets the specified setting to the specified value
         * @param name the name of the setting to set
         * @param value the value the setting is to be set to
         * @param index the index to set, default is zero
         * @return the new value of the setting specified by name
         */
        std::string Set(const std::string &name, const std::string &value, const size_t index = 0);
        /**
         * @brief Sets the specified setting to the specified value
         * @param name the name of the setting to set
         * @param value the value the setting is to be set to
         * @param index the index to set, default is zero
         * @return the new value of the setting specified by name
         */
        double Set(const std::string &name, const double &value, const size_t index = 0);

    private:
        template <typename T>
        T _Get(const std::string &name, const T &value, const size_t index);
        template <typename T>
        T _Set(const std::string &name, const T &value, const size_t index);
        using value_type = std::variant<double, std::string>;
        std::map<std::string, std::vector<value_type>, std::less<>> _configMap;
    };

    File &Global();

    template <typename T>
    inline T File::_Set(const std::string &name, const T &value, const size_t index)
    {
        auto &values = _configMap[name];
        values.resize((std::max)(index + 1, values.size()));
        values.at(index) = value;
        return std::get<T>(values.at(index));
    }
    inline double File::Set(const std::string &name, const double &value, const size_t index)
    {
        return _Set(name, value, index);
    }
    inline std::string File::Set(const std::string &name, const std::string &value, const size_t index)
    {
        return _Set(name, value, index);
    }

    template <typename T>
    inline T File::_Get(const std::string &name, const T &defaultValue, const size_t index)
    {
        auto it = _configMap.find(name);
        if (it != _configMap.end())
        {
            auto &values = it->second;
            values.resize((std::max)(index + 1, values.size()));
            return std::get<T>(values.at(index));
        }
        else
            return File::Set(name, defaultValue, index);
    }
    inline double File::Get(const std::string &name, const double &defaultValue, const size_t index)
    {
        return _Get(name, defaultValue, index);
    }
    inline std::string File::Get(const std::string &name, const std::string &defaultValue, const size_t index)
    {
        return _Get(name, defaultValue, index);
    }
}
