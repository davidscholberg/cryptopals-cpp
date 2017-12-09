#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <memory>
#include <string>
#include <vector>

namespace utils {
    std::shared_ptr<std::string> get_file_contents(const std::string &filename);
    std::shared_ptr<std::vector<std::string>> get_lines(const std::string &filename);
    std::shared_ptr<std::string> hexdump(const std::vector<unsigned char> &bytes);
}

#endif // UTILS_HPP_
