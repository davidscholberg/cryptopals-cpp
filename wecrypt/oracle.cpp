#include <memory>
#include <string>
#include <vector>

#include "wecrypt/cipher-mode.hpp"
#include "wecrypt/info.hpp"
#include "wecrypt/oracle.hpp"
#include "wecrypt/random.hpp"
#include "wecrypt/string-conversion.hpp"

namespace wecrypt {
    const std::string unknown_str("Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK");

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

    std::shared_ptr<std::vector<unsigned char>> ecb_encrypt_oracle(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer) {
        const auto unknown_buffer = base64_to_binary(unknown_str);
        if (!unknown_buffer) {
            return nullptr;
        }

        std::vector<unsigned char> full_buffer(
                buffer.size() + unknown_buffer->size());
        for (unsigned int i = 0; i < buffer.size(); i++) {
            full_buffer[i] = buffer[i];
        }
        for (unsigned int i = 0; i < unknown_buffer->size(); i++) {
            full_buffer[i + buffer.size()] = (*unknown_buffer)[i];
        }

        // we want the same key on every call per program run
        static const auto key_buffer = get_random_bytes(profile.block_size);

        return ecb_encrypt(profile, full_buffer, *key_buffer);
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

    // determines unknown string appended to plaintext in ecb_encrypt_oracle
    std::shared_ptr<std::vector<unsigned char>> attack_ecb_encrypt_oracle(
            const encryption_profile &profile) {
        // determine block size and unknown string size.
        // The idea here is that we will continue to add bytes to the plaintext
        // until the ciphertext output size changes. The difference between the
        // new and previous ciphertext lengths is the block size, and the
        // ciphertext length minus the plaintext length and the block size is
        // the size of the unknown string.
        auto prev_ciphertext = ecb_encrypt_oracle(
                profile,
                std::vector<unsigned char>());
        unsigned int block_size = 0;
        unsigned int plaintext_size = 0;
        unsigned int unknown_buffer_size = 0;
        while (block_size == 0) {
            plaintext_size++;
            auto cur_ciphertext = ecb_encrypt_oracle(
                    profile,
                    std::vector<unsigned char>(plaintext_size, 0));
            block_size = cur_ciphertext->size() - prev_ciphertext->size();
            unknown_buffer_size = cur_ciphertext->size() - plaintext_size - block_size;
        }

        // detect ecb mode. We should be able to create a buffer twice the
        // blocksize with identical bytes, and then see if any blocks of the
        // ciphertext are identical.
        auto ecb_ciphertext = ecb_encrypt_oracle(
                profile,
                std::vector<unsigned char>(block_size * 2, 0));
        if (count_identical_blocks(*ecb_ciphertext, block_size) < 1) {
            return nullptr;
        }

        // padding attack
        // round unknown_buffer_size up to nearest multiple of block_size
        unsigned int initial_padding_size =
            unknown_buffer_size - 1 + block_size - ((unknown_buffer_size - 1) % block_size);
        std::vector<unsigned char> padding(initial_padding_size, 0);
        std::vector<unsigned char> boundary_block(block_size, 0);
        unsigned int boundary_block_offset = initial_padding_size - block_size;
        auto unknown_buffer = std::make_shared<std::vector<unsigned char>>(unknown_buffer_size);
        for (unsigned int i = 0; i < unknown_buffer->size(); i++) {
            padding.pop_back();
            boundary_block.erase(boundary_block.begin());
            auto ciphertext = ecb_encrypt_oracle(profile, padding);
            unsigned char guessed_byte = 0;
            bool success = break_ecb_boundary_block(
                    profile,
                    boundary_block,
                    std::vector<unsigned char>(
                        ciphertext->begin() + boundary_block_offset,
                        ciphertext->begin() + boundary_block_offset + block_size),
                    block_size,
                    &guessed_byte);
            if (!success) {
                return nullptr;
            }
            boundary_block.push_back(guessed_byte);
            (*unknown_buffer)[i] = guessed_byte;
        }

        return unknown_buffer;
    }

    // determine byte at boundary block for ecb_encrypt_oracle
    bool break_ecb_boundary_block(
            const encryption_profile &profile,
            const std::vector<unsigned char> &plaintext_minus_one,
            const std::vector<unsigned char> &ciphertext,
            unsigned int block_size,
            unsigned char *byte) {
        if (!byte) {
            return false;
        }
        if (plaintext_minus_one.size() != block_size - 1) {
            return false;
        }
        if (ciphertext.size() != block_size) {
            return false;
        }

        std::vector<unsigned char> plaintext = plaintext_minus_one;
        plaintext.push_back(0);
        for (unsigned int i = 0; i < 256; i++) {
            *byte = i;
            plaintext[plaintext.size() - 1] = *byte;
            auto guessed_cipher_text = ecb_encrypt_oracle(profile, plaintext);
            // we need to strip off the padding block before comparing it to the
            // original ciphertext
            guessed_cipher_text->erase(
                    guessed_cipher_text->begin() + block_size,
                    guessed_cipher_text->end());
            if (*guessed_cipher_text == ciphertext) {
                return true;
            }
        }
        return false;
    }
}
