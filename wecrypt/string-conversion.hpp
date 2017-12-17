#ifndef STRING_CONVERSION_HPP_
#define STRING_CONVERSION_HPP_

#include <memory>
#include <string>
#include <vector>

namespace wecrypt {
    std::shared_ptr<std::string> hex_to_base64(const std::string &hex_str);
    std::shared_ptr<std::string> base64_to_hex(const std::string &hex_str);
    std::shared_ptr<std::vector<unsigned char>> base64_to_binary(const std::string &base64_str);
    std::shared_ptr<std::string> binary_to_base64(const std::vector<unsigned char> &buffer);
    std::shared_ptr<std::vector<unsigned char>> hex_to_binary(const std::string &hex_str);
    std::shared_ptr<std::string> binary_to_hex(const std::vector<unsigned char> &buffer);
    std::shared_ptr<std::string> binary_to_hex(const unsigned char byte);
}

#endif // STRING_CONVERSION_HPP_
