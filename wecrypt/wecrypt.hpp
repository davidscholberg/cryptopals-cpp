#ifndef WECRYPT_HPP_
#define WECRYPT_HPP_

#include <memory>
#include <string>
#include <vector>

namespace wecrypt {
    std::shared_ptr<std::string> hex_to_base64(const std::string &hex_str);
    std::shared_ptr<std::vector<unsigned char>> hex_to_binary(const std::string &hex_str);
    std::shared_ptr<std::string> binary_to_base64(const std::vector<unsigned char> &buffer);
    std::shared_ptr<std::string> binary_to_hex(const std::vector<unsigned char> &buffer);
    std::shared_ptr<std::string> binary_to_hex(const unsigned char byte);

    std::shared_ptr<std::vector<unsigned char>> xor_bytes(
            const std::vector<unsigned char> &buffer_a,
            const std::vector<unsigned char> &buffer_b);
    std::shared_ptr<std::vector<unsigned char>> xor_single_byte(
            const std::vector<unsigned char> &buffer,
            unsigned char byte);
    std::shared_ptr<std::vector<unsigned char>> xor_repeating_key(
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    struct xor_byte_score {
        float score;
        unsigned char byte;
        static bool compare(const xor_byte_score &i, const xor_byte_score &j);
        static bool rcompare(const xor_byte_score &i, const xor_byte_score &j);
    };
    float score_english(const std::vector<unsigned char> &buffer);
    std::shared_ptr<std::vector<xor_byte_score>> break_xor_single_byte(
            const std::vector<unsigned char> &buffer);

    struct xor_byte_scores {
        unsigned int index;
        std::shared_ptr<std::vector<xor_byte_score>> scores;
        static bool compare(const xor_byte_scores &i, const xor_byte_scores &j);
        static bool rcompare(const xor_byte_scores &i, const xor_byte_scores &j);
    };
    std::shared_ptr<std::vector<xor_byte_scores>> detect_xor_single_byte(
            const std::vector<std::vector<unsigned char>> &buffers);
}

#endif // WECRYPT_HPP_
