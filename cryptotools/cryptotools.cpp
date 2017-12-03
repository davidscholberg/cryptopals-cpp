#include "cryptotools/cryptotools.hpp"

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

    static const std::string hex_values("0123456789abcdef");
    auto buffer = std::make_shared<std::vector<unsigned char>>(hex_str->length() / 2);

    for (int i = 0; i < buffer->size(); i++) {
        int hex_str_index = i * 2;
        auto binary_val = hex_values.find((*hex_str)[hex_str_index]);
        if (binary_val == std::string::npos) {
            return nullptr;
        }
        (*buffer)[i] = binary_val << 4;

        hex_str_index++;
        binary_val = hex_values.find((*hex_str)[hex_str_index]);
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

    for (int i = 0; i < buffer->size(); i += 3) {
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
