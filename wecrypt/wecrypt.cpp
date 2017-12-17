#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "wecrypt/wecrypt.hpp"

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
    const std::unordered_map<char, unsigned char> base64_val_map = {
        {'A', 0},  {'B', 1},  {'C', 2},  {'D', 3},
        {'E', 4},  {'F', 5},  {'G', 6},  {'H', 7},
        {'I', 8},  {'J', 9},  {'K', 10}, {'L', 11},
        {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
        {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19},
        {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
        {'Y', 24}, {'Z', 25}, {'a', 26}, {'b', 27},
        {'c', 28}, {'d', 29}, {'e', 30}, {'f', 31},
        {'g', 32}, {'h', 33}, {'i', 34}, {'j', 35},
        {'k', 36}, {'l', 37}, {'m', 38}, {'n', 39},
        {'o', 40}, {'p', 41}, {'q', 42}, {'r', 43},
        {'s', 44}, {'t', 45}, {'u', 46}, {'v', 47},
        {'w', 48}, {'x', 49}, {'y', 50}, {'z', 51},
        {'0', 52}, {'1', 53}, {'2', 54}, {'3', 55},
        {'4', 56}, {'5', 57}, {'6', 58}, {'7', 59},
        {'8', 60}, {'9', 61}, {'+', 62}, {'/', 63},
    };
    const std::vector<char> hex_char_list = {
        {'0'}, {'1'}, {'2'}, {'3'},
        {'4'}, {'5'}, {'6'}, {'7'},
        {'8'}, {'9'}, {'a'}, {'b'},
        {'c'}, {'d'}, {'e'}, {'f'},
    };
    const std::unordered_map<char, unsigned char> hex_val_map = {
        {'0', 0},  {'1', 1},  {'2', 2},  {'3', 3},
        {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7},
        {'8', 8},  {'9', 9},  {'a', 10}, {'b', 11},
        {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
    };
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

            auto binary_val = base64_val_map.find(base64_str[i]);
            if (binary_val == base64_val_map.end()) {
                return nullptr;
            }

            unsigned int buffer_index = i * 3 / 4;

            switch (i % 4) {
                case 0:
                    (*buffer)[buffer_index] = binary_val->second << 2;
                    break;
                case 1:
                    (*buffer)[buffer_index] |= binary_val->second >> 4;
                    (*buffer)[buffer_index + 1] = binary_val->second << 4;
                    break;
                case 2:
                    (*buffer)[buffer_index] |= binary_val->second >> 2;
                    (*buffer)[buffer_index + 1] = binary_val->second << 6;
                    break;
                case 3:
                    (*buffer)[buffer_index] |= binary_val->second;
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

        for (unsigned int i = 0; i < buffer->size(); i++) {
            int hex_str_index = i * 2;
            auto binary_val = hex_val_map.find(hex_str[hex_str_index]);
            if (binary_val == hex_val_map.end()) {
                return nullptr;
            }
            (*buffer)[i] = binary_val->second << 4;

            hex_str_index++;
            binary_val = hex_val_map.find(hex_str[hex_str_index]);
            if (binary_val == hex_val_map.end()) {
                return nullptr;
            }
            (*buffer)[i] |= binary_val->second;
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

    /**
     * Returns nullptr if buffers don't have equal sizes.
     */
    std::shared_ptr<std::vector<unsigned char>> xor_bytes(
            const std::vector<unsigned char> &buffer_a,
            const std::vector<unsigned char> &buffer_b) {
        if (buffer_a.size() != buffer_b.size()) {
            return nullptr;
        }

        auto xor_buffer = std::make_shared<std::vector<unsigned char>>(buffer_a.size());

        for (unsigned int i = 0; i < xor_buffer->size(); i++) {
            (*xor_buffer)[i] = buffer_a[i] ^ buffer_b[i];
        }

        return xor_buffer;
    }

    std::shared_ptr<std::vector<unsigned char>> xor_single_byte(
            const std::vector<unsigned char> &buffer,
            unsigned char byte) {
        auto xor_buffer = std::make_shared<std::vector<unsigned char>>(buffer.size());

        for (unsigned int i = 0; i < xor_buffer->size(); i++) {
            (*xor_buffer)[i] = buffer[i] ^ byte;
        }

        return xor_buffer;
    }

    /**
     * Returns nullptr if key is larger than buffer.
     */
    std::shared_ptr<std::vector<unsigned char>> xor_repeating_key(
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key) {
        if (key.size() > buffer.size()) {
            return nullptr;
        }

        std::vector<unsigned char> key_buffer(buffer.size());

        for (unsigned int i = 0; i < key_buffer.size(); i++) {
            key_buffer[i] = key[i % key.size()];
        }

        return xor_bytes(buffer, key_buffer);
    }

    float score_english(const std::vector<unsigned char> &buffer) {
        float total_score = 0;

        for (unsigned int i = 0; i < buffer.size(); i++) {
            auto char_score = letter_freq.find(buffer[i]);

            if (char_score == letter_freq.end()) {
                continue;
            }

            total_score += char_score->second;
        }

        return total_score;
    }

    std::shared_ptr<std::vector<xor_byte_score>> break_xor_single_byte(
            const std::vector<unsigned char> &buffer) {
        auto scores = std::make_shared<std::vector<xor_byte_score>>(256);

        for (unsigned char i = 0; i < 255; i++) {
            auto xor_buffer = xor_single_byte(buffer, i);
            (*scores)[i].byte = i;
            (*scores)[i].score = score_english(*xor_buffer);
        }

        std::sort(scores->begin(), scores->end(), xor_byte_score::rcompare);

        return scores;
    }

    std::shared_ptr<std::vector<xor_byte_scores>> detect_xor_single_byte(
            const std::vector<std::vector<unsigned char>> &buffers) {
        auto scores = std::make_shared<std::vector<xor_byte_scores>>();

        for (unsigned int i = 0; i < buffers.size(); i++) {
            auto xor_scores = wecrypt::break_xor_single_byte(buffers[i]);
            scores->push_back({i, xor_scores});
        }

        std::sort(scores->begin(), scores->end(), xor_byte_scores::rcompare);

        return scores;
    }

    bool xor_byte_score::compare(const xor_byte_score &i, const xor_byte_score &j) {
        return i.score < j.score;
    }

    bool xor_byte_score::rcompare(const xor_byte_score &i, const xor_byte_score &j) {
        return j.score < i.score;
    }

    bool xor_byte_scores::compare(const xor_byte_scores &i, const xor_byte_scores &j) {
        if (!i.scores->empty() && !j.scores->empty()) {
            return xor_byte_score::compare((*i.scores)[0], (*j.scores)[0]);
        }
        return i.scores->empty();
    }

    bool xor_byte_scores::rcompare(const xor_byte_scores &i, const xor_byte_scores &j) {
        if (!i.scores->empty() && !j.scores->empty()) {
            return xor_byte_score::rcompare((*i.scores)[0], (*j.scores)[0]);
        }
        return j.scores->empty();
    }

    /**
     * Returns -1 if buffers don't have equal sizes.
     */
    int hamming_distance(
            const std::vector<unsigned char> &buffer_a,
            const std::vector<unsigned char> &buffer_b) {
        if (buffer_a.size() != buffer_b.size()) {
            return -1;
        }

        int distance = 0;

        for (unsigned int i = 0; i < buffer_a.size(); i++) {
            unsigned char xor_val = buffer_a[i] ^ buffer_b[i];

            // Kernigan's algorithm for counting bits set
            while (xor_val) {
                distance++;
                xor_val &= xor_val - 1;
            }
        }

        return distance;
    }

    std::shared_ptr<std::vector<unsigned char>> break_xor_repeating_key(
            const std::vector<unsigned char> &buffer) {
        unsigned int key_size_guess = 0;
        // This is the average hamming distance of keysized chunks of the buffer
        // normalized by the keysize.
        // The initial value is the largest hamming distance possible for the
        // key sizes we're checking.
        float smallest_norm_distance = std::max((int)(buffer.size() / 2), 40) * 8;
        for (unsigned int i = 2; i <= buffer.size() / 2 && i < 40; i++) {
            int current_distance = 0;
            for (unsigned int j = 0; j < (buffer.size() / i) - 1; j++) {
                std::vector<unsigned char> buffer_a(
                        buffer.begin() + (j * i),
                        buffer.begin() + ((j + 1) * i));
                std::vector<unsigned char> buffer_b(
                        buffer.begin() + ((j + 1) * i),
                        buffer.begin() + ((j + 2) * i));
                current_distance += hamming_distance(buffer_a, buffer_b);
            }
            float norm_distance = (float)current_distance / ((buffer.size() / (float)i) - 1) / (float)i;
            if (norm_distance < smallest_norm_distance) {
                smallest_norm_distance = norm_distance;
                key_size_guess = i;
            }
        }

        auto key_buffer = std::make_shared<std::vector<unsigned char>>(key_size_guess);
        for (unsigned int i = 0; i < key_size_guess; i++) {
            std::vector<unsigned char> transposed_buffer(((buffer.size() - i - 1) / key_size_guess) + 1);
            for (unsigned int j = 0; j < transposed_buffer.size(); j++) {
                transposed_buffer[j] = buffer[(j * key_size_guess) + i];
            }
            auto scores = break_xor_single_byte(transposed_buffer);
            (*key_buffer)[i] = (*scores)[0].byte;
        }

        return key_buffer;
    }
}
