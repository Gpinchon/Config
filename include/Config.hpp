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
         * @tparam T the type that we expect
         * @param name the name of the setting
         * @param defaultValue the default value the setting is to be set to if it is non-existent
         * @param index the index to query, default is zero
         * @return the value of the setting specified by name
         */
        template <typename T>
        T Get(const std::string &name, const T defaultValue, const size_t index = 0);
        /**
         * @brief Sets the specified setting to the specified value
         * @tparam T the type of this setting
         * @param name the name of the setting to set
         * @param value the value the setting is to be set to
         * @param index the index to set, default is zero
         * @return the new value of the setting specified by name
         */
        template <typename T>
        T Set(const std::string &name, const T value, const size_t index = 0);

    private:
        typedef std::variant<double, std::string> value_type;
        std::map<std::string, std::vector<value_type>> _configMap;
    };

    File &Global();

    template <>
    inline float File::Get<float>(const std::string &name, const float defaultValue, const size_t index)
    {
        return float(Get<double>(name, double(defaultValue), index));
    }

    template <>
    inline short File::Get<short>(const std::string &name, const short defaultValue, const size_t index)
    {
        return short(Get<double>(name, double(defaultValue), index));
    }

    template <>
    inline unsigned short File::Get<unsigned short>(const std::string &name, const unsigned short defaultValue, const size_t index)
    {
        return (unsigned short)(Get<double>(name, double(defaultValue), index));
    }

    template <>
    inline int File::Get<int>(const std::string &name, const int defaultValue, const size_t index)
    {
        return int(Get<double>(name, double(defaultValue), index));
    }

    template <>
    inline unsigned File::Get<unsigned>(const std::string &name, const unsigned defaultValue, const size_t index)
    {
        return unsigned(Get<double>(name, double(defaultValue), index));
    }

    template <typename T>
    inline T File::Get(const std::string &name, const T defaultValue, const size_t index)
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

    template <typename T>
    inline T File::Set(const std::string &name, const T value, const size_t index)
    {
        auto &values = _configMap[name];
        values.resize((std::max)(index + 1, values.size()));
        return std::get<T>(values.at(index) = value);
    }
}
