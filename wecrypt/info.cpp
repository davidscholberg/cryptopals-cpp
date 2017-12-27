#include <unordered_map>
#include <vector>

#include "wecrypt/info.hpp"

namespace wecrypt {
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

    // get average hamming distance between all pairs of blocks in buffer
    // returns -1 if block_size is more than half of the buffer size
    float avg_hamming_distance(
            const std::vector<unsigned char> &buffer,
            const unsigned int block_size) {
        if (buffer.size() / 2 < block_size) {
            return -1;
        }

        unsigned int blocks = buffer.size() / block_size;
        int total_distance = 0;
        for (unsigned int i = 0; i < blocks - 1; i++) {
            std::vector<unsigned char> buffer_a(
                    buffer.begin() + (i * block_size),
                    buffer.begin() + ((i + 1) * block_size));
            for (unsigned int j = i + 1; j < blocks; j++) {
                std::vector<unsigned char> buffer_b(
                        buffer.begin() + (j * block_size),
                        buffer.begin() + ((j + 1) * block_size));
                total_distance += hamming_distance(buffer_a, buffer_b);
            }
        }
        // the number of iterations is n choose 2, where n is the number of full
        // block_size blocks in buffer
        unsigned int iterations = (blocks * (blocks - 1)) / 2;
        return (float)total_distance / iterations;
    }

    // get count of all identical block pairs in buffer
    // returns -1 if block_size is more than half of the buffer size
    int count_identical_blocks(
            const std::vector<unsigned char> &buffer,
            const unsigned int block_size) {
        if (buffer.size() / 2 < block_size) {
            return -1;
        }

        unsigned int blocks = buffer.size() / block_size;
        int identical_blocks = 0;
        for (unsigned int i = 0; i < blocks - 1; i++) {
            std::vector<unsigned char> buffer_a(
                    buffer.begin() + (i * block_size),
                    buffer.begin() + ((i + 1) * block_size));
            for (unsigned int j = i + 1; j < blocks; j++) {
                std::vector<unsigned char> buffer_b(
                        buffer.begin() + (j * block_size),
                        buffer.begin() + ((j + 1) * block_size));
                if(hamming_distance(buffer_a, buffer_b) == 0) {
                    identical_blocks++;
                }
            }
        }

        return identical_blocks;
    }
}
