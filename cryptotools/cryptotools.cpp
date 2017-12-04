#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "cryptotools/cryptotools.hpp"

namespace cryptotools {
    const std::string hex_values("0123456789abcdef");
    const std::unordered_map<char, float> letter_freq = {
        {'a', 8.167},
        {'b', 1.492},
        {'c', 2.782},
        {'d', 4.253},
        {'e', 12.702},
        {'f', 2.228},
        {'g', 2.015},
        {'h', 6.094},
        {'i', 6.966},
        {'j', 0.153},
        {'k', 0.772},
        {'l', 4.025},
        {'m', 2.406},
        {'n', 6.749},
        {'o', 7.507},
        {'p', 1.929},
        {'q', 0.095},
        {'r', 5.987},
        {'s', 6.327},
        {'t', 9.056},
        {'u', 2.758},
        {'v', 0.978},
        {'w', 2.360},
        {'x', 0.150},
        {'y', 1.974},
        {'z', 0.074},
        {' ', 17.647},
    };
}

std::shared_ptr<std::string> cryptotools::hex_to_base64(const std::shared_ptr<std::string> hex_str) {
    return binary_to_base64(hex_to_binary(hex_str));
}

/**
 * Expects hex_str length to be even and contain valid hex digits.
 * Returns nullptr if not or if hex_str is a nullptr.
 */
std::shared_ptr<std::vector<unsigned char>> cryptotools::hex_to_binary(const std::shared_ptr<std::string> hex_str) {
    if (!hex_str) {
        return nullptr;
    }
    if (hex_str->length() % 2 != 0) {
        return nullptr;
    }

    auto buffer = std::make_shared<std::vector<unsigned char>>(hex_str->length() / 2);

    for (unsigned int i = 0; i < buffer->size(); i++) {
        int hex_str_index = i * 2;
        auto binary_val = cryptotools::hex_values.find((*hex_str)[hex_str_index]);
        if (binary_val == std::string::npos) {
            return nullptr;
        }
        (*buffer)[i] = binary_val << 4;

        hex_str_index++;
        binary_val = cryptotools::hex_values.find((*hex_str)[hex_str_index]);
        if (binary_val == std::string::npos) {
            return nullptr;
        }
        (*buffer)[i] |= binary_val;
    }

    return buffer;
}

/**
 * Returns nullptr if buffer is a nullptr.
 */
std::shared_ptr<std::string> cryptotools::binary_to_base64(const std::shared_ptr<std::vector<unsigned char>> buffer) {
    if (!buffer) {
        return nullptr;
    }

    static const std::string base64_values("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    static const char padding_char = '=';

    // base64 string size is buffer size * 3/4 rounded up to next multiple of 4
    const unsigned int base64_str_len = (((buffer->size() * 4 / 3) + 4 - 1) / 4) * 4;
    auto base64_str = std::make_shared<std::string>(base64_str_len, padding_char);

    int base64_str_index = 0;

    for (unsigned int i = 0; i < buffer->size(); i += 3) {
        // first 6-bit group
        char binary_val = (*buffer)[i] >> 2;
        (*base64_str)[base64_str_index] = base64_values[binary_val];

        // second 6-bit group
        base64_str_index++;
        binary_val = ((*buffer)[i] << 4) & 0b00110000;
        if (i + 1 < buffer->size()) {
            binary_val |= (*buffer)[i + 1] >> 4;
        }
        (*base64_str)[base64_str_index] = base64_values[binary_val];
        if (i + 1 >= buffer->size()) {
            break;
        }

        // third 6-bit group
        base64_str_index++;
        binary_val = ((*buffer)[i + 1] << 2) & 0b00111100;
        if (i + 2 < buffer->size()) {
            binary_val |= (*buffer)[i + 2] >> 6;
        }
        (*base64_str)[base64_str_index] = base64_values[binary_val];
        if (i + 2 >= buffer->size()) {
            break;
        }

        // fourth 6-bit group
        base64_str_index++;
        binary_val = (*buffer)[i + 2] & 0b00111111;
        (*base64_str)[base64_str_index] = base64_values[binary_val];

        base64_str_index++;
    }

    return base64_str;
}

/**
 * Returns nullptr if buffer is a nullptr.
 */
std::shared_ptr<std::string> cryptotools::binary_to_hex(const std::shared_ptr<std::vector<unsigned char>> buffer) {
    if (!buffer) {
        return nullptr;
    }

    auto hex_str = std::make_shared<std::string>(buffer->size() * 2, '0');

    for (unsigned int i = 0; i < buffer->size(); i++) {
        // first 4-bit group
        char binary_val = (*buffer)[i] >> 4;
        (*hex_str)[i * 2] = cryptotools::hex_values[binary_val];

        // second 4-bit group
        binary_val = (*buffer)[i] & 0b00001111;
        (*hex_str)[i * 2 + 1] = cryptotools::hex_values[binary_val];
    }

    return hex_str;
}

std::shared_ptr<std::string> cryptotools::binary_to_hex(const unsigned char byte) {
    auto hex_str = std::make_shared<std::string>(2, '0');

    // first 4-bit group
    char binary_val = byte >> 4;
    (*hex_str)[0] = cryptotools::hex_values[binary_val];

    // second 4-bit group
    binary_val = byte & 0b00001111;
    (*hex_str)[1] = cryptotools::hex_values[binary_val];

    return hex_str;
}

/**
 * Returns nullptr if either buffer is a nullptr or if they don't have equal sizes.
 */
std::shared_ptr<std::vector<unsigned char>> cryptotools::xor_bytes(
        const std::shared_ptr<std::vector<unsigned char>> buffer_a,
        const std::shared_ptr<std::vector<unsigned char>> buffer_b) {
    if (!buffer_a || !buffer_b) {
        return nullptr;
    }
    if (buffer_a->size() != buffer_b->size()) {
        return nullptr;
    }

    auto xor_buffer = std::make_shared<std::vector<unsigned char>>(buffer_a->size());

    for (unsigned int i = 0; i < xor_buffer->size(); i++) {
        (*xor_buffer)[i] = (*buffer_a)[i] ^ (*buffer_b)[i];
    }

    return xor_buffer;
}

/**
 * Returns nullptr if buffer is a nullptr.
 */
std::shared_ptr<std::vector<unsigned char>> cryptotools::xor_single_byte(
        const std::shared_ptr<std::vector<unsigned char>> buffer,
        unsigned char byte) {
    if (!buffer) {
        return nullptr;
    }

    auto xor_buffer = std::make_shared<std::vector<unsigned char>>(buffer->size());

    for (unsigned int i = 0; i < xor_buffer->size(); i++) {
        (*xor_buffer)[i] = (*buffer)[i] ^ byte;
    }

    return xor_buffer;
}

/**
 * Returns -1 if buffer is a nullptr.
 */
float cryptotools::score_english(const std::shared_ptr<std::vector<unsigned char>> buffer) {
    if (!buffer) {
        return -1;
    }

    float total_score = 0;

    for (unsigned int i = 0; i < buffer->size(); i++) {
        auto char_score = cryptotools::letter_freq.find((*buffer)[i]);

        if (char_score == cryptotools::letter_freq.end()) {
            continue;
        }

        total_score += char_score->second;
    }

    return total_score;
}

/**
 * Returns nullptr if buffer is a nullptr.
 */
std::shared_ptr<std::vector<cryptotools::xor_byte_score>> cryptotools::break_xor_single_byte(
        const std::shared_ptr<std::vector<unsigned char>> buffer) {
    if (!buffer) {
        return nullptr;
    }

    auto scores = std::make_shared<std::vector<cryptotools::xor_byte_score>>(256);

    for (unsigned char i = 0; i < 255; i++) {
        auto xor_buffer = cryptotools::xor_single_byte(buffer, i);
        (*scores)[i].byte = i;
        (*scores)[i].score = cryptotools::score_english(xor_buffer);
    }

    std::sort(scores->begin(), scores->end(), cryptotools::xor_byte_score::rcompare);

    return scores;
}

bool cryptotools::xor_byte_score::compare(
        cryptotools::xor_byte_score i,
        cryptotools::xor_byte_score j) {
    return i.score < j.score;
}

bool cryptotools::xor_byte_score::rcompare(
        cryptotools::xor_byte_score i,
        cryptotools::xor_byte_score j) {
    return j.score < i.score;
}
