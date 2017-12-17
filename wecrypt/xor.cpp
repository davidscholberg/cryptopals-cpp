#include <algorithm>
#include <memory>
#include <vector>

#include "wecrypt/info.hpp"
#include "wecrypt/xor.hpp"

namespace wecrypt {
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
