#ifndef CRYPTOTOOLS_HPP_
#define CRYPTOTOOLS_HPP_

#include <memory>
#include <string>
#include <vector>

namespace cryptotools {
    std::shared_ptr<std::string> hex_to_base64(const std::shared_ptr<std::string> hex_str);
    std::shared_ptr<std::vector<unsigned char>> hex_to_binary(const std::shared_ptr<std::string> hex_str);
    std::shared_ptr<std::string> binary_to_base64(const std::shared_ptr<std::vector<unsigned char>> buffer);
    std::shared_ptr<std::string> binary_to_hex(const std::shared_ptr<std::vector<unsigned char>> buffer);

    std::shared_ptr<std::vector<unsigned char>> xor_bytes(
            const std::shared_ptr<std::vector<unsigned char>> buffer_a,
            const std::shared_ptr<std::vector<unsigned char>> buffer_b);
}

#endif // CRYPTOTOOLS_HPP_
