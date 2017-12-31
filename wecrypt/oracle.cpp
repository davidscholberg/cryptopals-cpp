#include <algorithm>
#include <memory>
#include <vector>

#include "wecrypt/cipher-mode.hpp"
#include "wecrypt/info.hpp"
#include "wecrypt/oracle.hpp"
#include "wecrypt/random.hpp"

namespace wecrypt {
    std::shared_ptr<std::vector<unsigned char>> ecb_cbc_encrypt_oracle(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer) {
        rng_ints rng_padding_size(5, 10);

        auto pre_padding = get_random_bytes(rng_padding_size());
        auto post_padding = get_random_bytes(rng_padding_size());
        std::vector<unsigned char> padded_buffer(
                pre_padding->size() + buffer.size() + post_padding->size());
        for (unsigned int i = 0; i < pre_padding->size(); i++) {
            padded_buffer[i] = (*pre_padding)[i];
        }
        for (unsigned int i = 0; i < buffer.size(); i++) {
            padded_buffer[i + pre_padding->size()] = buffer[i];
        }
        for (unsigned int i = 0; i < post_padding->size(); i++) {
            padded_buffer[i + pre_padding->size() + buffer.size()] = (*post_padding)[i];
        }

        auto key_buffer = get_random_bytes(profile.block_size);

        static rng_ints rng_boolean(0, 1);

        if (rng_boolean()) {
            return ecb_encrypt(profile, padded_buffer, *key_buffer);
        }

        auto iv = get_random_bytes(profile.block_size);
        return cbc_encrypt(profile, padded_buffer, *key_buffer, *iv);
    }

    // detects cipher mode used by ecb_cbc_encrypt_oracle
    cipher_mode attack_ecb_cbc_encrypt_oracle(
            const encryption_profile &profile) {
        // The minimum number of bytes prepended by random_ecb_cbc_encrypt is
        // 5, and the max is 10. Therefore, we need a 43 byte buffer of
        // identical bytes in order to ensure 2 full identical blocks in the
        // padded buffer.
        std::vector<unsigned char> buffer(43, 0);

        auto ciphertext = ecb_cbc_encrypt_oracle(profile, buffer);

        switch(count_identical_blocks(*ciphertext, profile.block_size)) {
            case 0:
                return cipher_mode::cbc;
            case 1:
                return cipher_mode::ecb;
            default:
                return cipher_mode::error;
        }
    }
}
