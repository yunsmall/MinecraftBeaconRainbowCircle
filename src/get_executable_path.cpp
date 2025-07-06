#include "utils.h"

#include <string>
#include <system_error>
// Windows平台头文件
#if defined(_WIN32)
#include <windows.h>
// Unix-like平台头文件（包括Linux和macOS）
#else
#include <unistd.h>
#include <climits>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#endif
std::filesystem::path get_executable_path() {
#if defined(_WIN32)
    // Windows实现
    wchar_t buffer[MAX_PATH];
    DWORD result = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    if (result == 0 || result == MAX_PATH) {
        return {}; // 失败时返回空路径
    }
    return std::filesystem::path(buffer).lexically_normal();
#elif defined(__APPLE__)
    // macOS实现
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string path(size, '\0');
    if (_NSGetExecutablePath(path.data(), &size) != 0) {
        return {}; // 缓冲区不足时返回空
    }
    path.resize(size - 1); // 移除终止null字符
    return std::filesystem::absolute(path).lexically_normal();
#else
    // Linux及其他Unix-like系统实现
    std::string path(PATH_MAX, '\0');
    ssize_t count = readlink("/proc/self/exe", path.data(), PATH_MAX);
    if (count < 0) {
        return {}; // readlink失败
    }
    path.resize(count);
    return std::filesystem::absolute(path).lexically_normal();
#endif
}