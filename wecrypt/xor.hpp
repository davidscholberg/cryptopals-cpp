#ifndef XOR_HPP_
#define XOR_HPP_

#include <memory>
#include <vector>

namespace wecrypt {
    struct xor_byte_score {
        float score;
        unsigned char byte;
        static bool compare(const xor_byte_score &i, const xor_byte_score &j);
        static bool rcompare(const xor_byte_score &i, const xor_byte_score &j);
    };

    struct xor_byte_scores {
        unsigned int index;
        std::shared_ptr<std::vector<xor_byte_score>> scores;
        static bool compare(const xor_byte_scores &i, const xor_byte_scores &j);
        static bool rcompare(const xor_byte_scores &i, const xor_byte_scores &j);
    };

    std::shared_ptr<std::vector<unsigned char>> xor_bytes(
            const std::vector<unsigned char> &buffer_a,
            const std::vector<unsigned char> &buffer_b);

    std::shared_ptr<std::vector<unsigned char>> xor_single_byte(
            const std::vector<unsigned char> &buffer,
            unsigned char byte);

    std::shared_ptr<std::vector<unsigned char>> xor_repeating_key(
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    std::shared_ptr<std::vector<xor_byte_score>> break_xor_single_byte(
            const std::vector<unsigned char> &buffer);

    std::shared_ptr<std::vector<xor_byte_scores>> detect_xor_single_byte(
            const std::vector<std::vector<unsigned char>> &buffers);

    std::shared_ptr<std::vector<unsigned char>> break_xor_repeating_key(
            const std::vector<unsigned char> &buffer);
}

#endif // XOR_HPP_
