#include <memory>
#include <string>
#include <vector>

#include "wecrypt/string-conversion.hpp"

namespace wecrypt {
    const std::vector<char> base64_char_list = {
        {'A'}, {'B'}, {'C'}, {'D'},
        {'E'}, {'F'}, {'G'}, {'H'},
        {'I'}, {'J'}, {'K'}, {'L'},
        {'M'}, {'N'}, {'O'}, {'P'},
        {'Q'}, {'R'}, {'S'}, {'T'},
        {'U'}, {'V'}, {'W'}, {'X'},
        {'Y'}, {'Z'}, {'a'}, {'b'},
        {'c'}, {'d'}, {'e'}, {'f'},
        {'g'}, {'h'}, {'i'}, {'j'},
        {'k'}, {'l'}, {'m'}, {'n'},
        {'o'}, {'p'}, {'q'}, {'r'},
        {'s'}, {'t'}, {'u'}, {'v'},
        {'w'}, {'x'}, {'y'}, {'z'},
        {'0'}, {'1'}, {'2'}, {'3'},
        {'4'}, {'5'}, {'6'}, {'7'},
        {'8'}, {'9'}, {'+'}, {'/'},
    };
    const char base64_pad_char = '=';
    const std::vector<char> hex_char_list = {
        {'0'}, {'1'}, {'2'}, {'3'},
        {'4'}, {'5'}, {'6'}, {'7'},
        {'8'}, {'9'}, {'a'}, {'b'},
        {'c'}, {'d'}, {'e'}, {'f'},
    };

    std::shared_ptr<std::string> hex_to_base64(const std::string &hex_str) {
        auto buffer = hex_to_binary(hex_str);
        if (!buffer) {
            return nullptr;
        }
        return binary_to_base64(*buffer);
    }

    std::shared_ptr<std::string> base64_to_hex(const std::string &base64_str) {
        auto buffer = base64_to_binary(base64_str);
        if (!buffer) {
            return nullptr;
        }
        return binary_to_hex(*buffer);
    }

    /**
     * Returns nullptr if base64_str is invalid base64.
     */
    std::shared_ptr<std::vector<unsigned char>> base64_to_binary(const std::string &base64_str) {
        if (base64_str.length() % 4 != 0) {
            return nullptr;
        }

        if (base64_str.length() == 0) {
            return std::make_shared<std::vector<unsigned char>>();
        }

        unsigned int padding_chars = 0;

        for (unsigned int i = base64_str.length() - 1; i > base64_str.length() - 3; i--) {
            if (base64_str[i] != base64_pad_char) {
                break;
            }
            padding_chars++;
        }

        // the buffer size is the string size * 3/4 minus the number of padding chars
        auto buffer = std::make_shared<std::vector<unsigned char>>(
                (base64_str.length() * 3 / 4) - padding_chars);

        for (unsigned int i = 0; i < base64_str.length(); i++) {
            if (base64_str[i] == base64_pad_char) {
                if (i < base64_str.length() - 2) {
                    return nullptr;
                }
                if (i == base64_str.length() - 2 && base64_str[i + 1] != base64_pad_char) {
                    return nullptr;
                }
                break;
            }

            unsigned char binary_val = 0;
            const char base64_char = base64_str[i];
            if (base64_char >= 'A' && base64_char <= 'Z') {
                binary_val = base64_char - 65;
            } else if (base64_char >= 'a' && base64_char <= 'z') {
                binary_val = base64_char - 71;
            } else if (base64_char >= '0' && base64_char <= '9') {
                binary_val = base64_char + 4;
            } else if (base64_char == '+') {
                binary_val = 62;
            } else if (base64_char == '/') {
                binary_val = 63;
            } else {
                return nullptr;
            }

            const unsigned int buffer_index = i * 3 / 4;

            switch (i % 4) {
                case 0:
                    (*buffer)[buffer_index] = binary_val << 2;
                    break;
                case 1:
                    (*buffer)[buffer_index] |= binary_val >> 4;
                    (*buffer)[buffer_index + 1] = binary_val << 4;
                    break;
                case 2:
                    (*buffer)[buffer_index] |= binary_val >> 2;
                    (*buffer)[buffer_index + 1] = binary_val << 6;
                    break;
                case 3:
                    (*buffer)[buffer_index] |= binary_val;
                    break;
            }
        }

        return buffer;
    }

    std::shared_ptr<std::string> binary_to_base64(const std::vector<unsigned char> &buffer) {
        // base64 string size is buffer size * 4/3 rounded up to next multiple of 4
        const unsigned int base64_str_len = (((buffer.size() * 4 / 3) + 4 - 1) / 4) * 4;
        auto base64_str = std::make_shared<std::string>(base64_str_len, base64_pad_char);

        int base64_str_index = 0;

        for (unsigned int i = 0; i < buffer.size(); i += 3) {
            // first 6-bit group
            char binary_val = buffer[i] >> 2;
            (*base64_str)[base64_str_index] = base64_char_list[binary_val];

            // second 6-bit group
            base64_str_index++;
            binary_val = (buffer[i] << 4) & 0b00110000;
            if (i + 1 < buffer.size()) {
                binary_val |= buffer[i + 1] >> 4;
            }
            (*base64_str)[base64_str_index] = base64_char_list[binary_val];
            if (i + 1 >= buffer.size()) {
                break;
            }

            // third 6-bit group
            base64_str_index++;
            binary_val = (buffer[i + 1] << 2) & 0b00111100;
            if (i + 2 < buffer.size()) {
                binary_val |= buffer[i + 2] >> 6;
            }
            (*base64_str)[base64_str_index] = base64_char_list[binary_val];
            if (i + 2 >= buffer.size()) {
                break;
            }

            // fourth 6-bit group
            base64_str_index++;
            binary_val = buffer[i + 2] & 0b00111111;
            (*base64_str)[base64_str_index] = base64_char_list[binary_val];

            base64_str_index++;
        }

        return base64_str;
    }

    /**
     * Expects hex_str length to be even and contain valid hex digits.
     * Returns nullptr if not.
     */
    std::shared_ptr<std::vector<unsigned char>> hex_to_binary(const std::string &hex_str) {
        if (hex_str.length() % 2 != 0) {
            return nullptr;
        }

        auto buffer = std::make_shared<std::vector<unsigned char>>(hex_str.length() / 2);

        for (unsigned int i = 0; i < hex_str.length(); i++) {
            unsigned char binary_val = 0;
            const char hex_char = hex_str[i];
            if (hex_char >= '0' && hex_char <= '9') {
                binary_val = hex_char - 48;
            } else if (hex_char >= 'a' && hex_char <= 'f') {
                binary_val = hex_char - 87;
            } else if (hex_char >= 'A' && hex_char <= 'F') {
                binary_val = hex_char - 55;
            } else {
                return nullptr;
            }

            const unsigned int buffer_index = i / 2;

            switch (i % 2) {
                case 0:
                    (*buffer)[buffer_index] = binary_val << 4;
                    break;
                case 1:
                    (*buffer)[buffer_index] |= binary_val;
                    break;
            }
        }

        return buffer;
    }

    std::shared_ptr<std::string> binary_to_hex(const std::vector<unsigned char> &buffer) {
        auto hex_str = std::make_shared<std::string>(buffer.size() * 2, '0');

        for (unsigned int i = 0; i < buffer.size(); i++) {
            // first 4-bit group
            char binary_val = buffer[i] >> 4;
            (*hex_str)[i * 2] = hex_char_list[binary_val];

            // second 4-bit group
            binary_val = buffer[i] & 0b00001111;
            (*hex_str)[i * 2 + 1] = hex_char_list[binary_val];
        }

        return hex_str;
    }

    std::shared_ptr<std::string> binary_to_hex(const unsigned char byte) {
        auto hex_str = std::make_shared<std::string>(2, '0');

        // first 4-bit group
        char binary_val = byte >> 4;
        (*hex_str)[0] = hex_char_list[binary_val];

        // second 4-bit group
        binary_val = byte & 0b00001111;
        (*hex_str)[1] = hex_char_list[binary_val];

        return hex_str;
    }
}
