#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <memory>
#include <string>
#include <vector>

namespace utils {
    std::shared_ptr<std::string> get_file_contents(std::shared_ptr<std::string> filename);
    std::shared_ptr<std::vector<std::string>> get_lines(std::shared_ptr<std::string> filename);
    std::shared_ptr<std::string> hexdump(std::shared_ptr<std::vector<unsigned char>> bytes);
}

#endif // UTILS_HPP_
