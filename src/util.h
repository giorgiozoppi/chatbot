#ifndef CHAT_UTIL_H
#define CHAT_UTIL_H
#include <string>
#include <filesystem>
///
/// @brief This fixes the path to be relative from the root directory
///  instead of the build directory.
inline std::string BasePath()
{
    auto path{std::filesystem::current_path()};
    path += "/images/";
    if (std::filesystem::is_directory(path))
    {
        return "./";
    }
    return "../";
}
#endif