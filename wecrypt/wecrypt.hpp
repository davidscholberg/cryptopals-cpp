#ifndef WECRYPT_HPP_
#define WECRYPT_HPP_

#include <memory>
#include <string>
#include <vector>

namespace wecrypt {
    std::shared_ptr<std::string> hex_to_base64(const std::shared_ptr<std::string> hex_str);
    std::shared_ptr<std::vector<unsigned char>> hex_to_binary(const std::shared_ptr<std::string> hex_str);
    std::shared_ptr<std::string> binary_to_base64(const std::shared_ptr<std::vector<unsigned char>> buffer);
    std::shared_ptr<std::string> binary_to_hex(const std::shared_ptr<std::vector<unsigned char>> buffer);
    std::shared_ptr<std::string> binary_to_hex(const unsigned char byte);

    std::shared_ptr<std::vector<unsigned char>> xor_bytes(
            const std::shared_ptr<std::vector<unsigned char>> buffer_a,
            const std::shared_ptr<std::vector<unsigned char>> buffer_b);
    std::shared_ptr<std::vector<unsigned char>> xor_single_byte(
            const std::shared_ptr<std::vector<unsigned char>> buffer,
            unsigned char byte);

    struct xor_byte_score {
        float score;
        unsigned char byte;
        static bool compare(xor_byte_score i, xor_byte_score j);
        static bool rcompare(xor_byte_score i, xor_byte_score j);
    };
    float score_english(const std::shared_ptr<std::vector<unsigned char>> buffer);
    std::shared_ptr<std::vector<xor_byte_score>> break_xor_single_byte(
            const std::shared_ptr<std::vector<unsigned char>> buffer);

    struct xor_byte_scores {
        unsigned int index;
        std::shared_ptr<std::vector<xor_byte_score>> scores;
        static bool compare(xor_byte_scores i, xor_byte_scores j);
        static bool rcompare(xor_byte_scores i, xor_byte_scores j);
    };
    std::shared_ptr<std::vector<xor_byte_scores>> detect_xor_single_byte(
            const std::shared_ptr<std::vector<std::string>> hex_strs);
}

#endif // WECRYPT_HPP_
