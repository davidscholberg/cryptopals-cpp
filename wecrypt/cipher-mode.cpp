#include <algorithm>
#include <memory>
#include <vector>

#include "wecrypt/aes.hpp"
#include "wecrypt/cipher-mode.hpp"
#include "wecrypt/info.hpp"
#include "wecrypt/padding.hpp"
#include "wecrypt/random.hpp"
#include "wecrypt/xor.hpp"

namespace wecrypt {
    const encryption_profile aes_pkcs7_encrypt = {
        wecrypt::aes_encrypt,
        wecrypt::pkcs7_pad,
        aes_block_size
    };

    const decryption_profile aes_pkcs7_decrypt = {
        wecrypt::aes_decrypt,
        wecrypt::pkcs7_unpad,
        aes_block_size
    };

    std::shared_ptr<std::vector<unsigned char>> cbc_encrypt(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key,
            const std::vector<unsigned char> &iv) {
        if (iv.size() != profile.block_size) {
            return nullptr;
        }

        const auto padded_buffer = profile.pad(buffer, profile.block_size);

        auto ciphertext = std::make_shared<std::vector<unsigned char>>(padded_buffer->size());

        auto ciphertext_block = std::make_shared<std::vector<unsigned char>>(iv);

        for (unsigned int i = 0; i < padded_buffer->size(); i += profile.block_size) {
            const std::vector<unsigned char> plaintext_block(
                padded_buffer->begin() + i,
                padded_buffer->begin() + i + profile.block_size);

            const auto xored_block = xor_bytes(plaintext_block, *ciphertext_block);
            if (!xored_block) {
                return nullptr;
            }

            ciphertext_block = profile.encrypt(*xored_block, key);
            if (!ciphertext_block) {
                return nullptr;
            }

            for (unsigned int j = 0; j < ciphertext_block->size(); j++) {
                (*ciphertext)[i + j] = (*ciphertext_block)[j];
            }
        }

        return ciphertext;
    }

    std::shared_ptr<std::vector<unsigned char>> cbc_decrypt(
            const decryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key,
            const std::vector<unsigned char> &iv) {
        if (buffer.size() % profile.block_size != 0) {
            return nullptr;
        }
        if (iv.size() != profile.block_size) {
            return nullptr;
        }

        auto plaintext = std::make_shared<std::vector<unsigned char>>(buffer.size());

        std::vector<unsigned char> prev_ciphertext_block(iv);

        for (unsigned int i = 0; i < buffer.size(); i += profile.block_size) {
            const std::vector<unsigned char> ciphertext_block(
                buffer.begin() + i,
                buffer.begin() + i + profile.block_size);

            const auto xored_block = profile.decrypt(ciphertext_block, key);
            if (!xored_block) {
                return nullptr;
            }

            const auto plaintext_block = xor_bytes(*xored_block, prev_ciphertext_block);
            if (!plaintext_block) {
                return nullptr;
            }

            for (unsigned int j = 0; j < plaintext_block->size(); j++) {
                (*plaintext)[i + j] = (*plaintext_block)[j];
            }

            prev_ciphertext_block = ciphertext_block;
        }

        const bool ok = profile.unpad(*plaintext, profile.block_size);
        if (!ok) {
            return nullptr;
        }

        return plaintext;
    }

    std::shared_ptr<std::vector<unsigned char>> ecb_encrypt(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key) {
        const auto padded_buffer = profile.pad(buffer, profile.block_size);

        auto ciphertext = std::make_shared<std::vector<unsigned char>>(padded_buffer->size());

        for (unsigned int i = 0; i < padded_buffer->size(); i += profile.block_size) {
            const auto ciphertext_block = profile.encrypt(
                    std::vector<unsigned char>(
                        padded_buffer->begin() + i,
                        padded_buffer->begin() + i + profile.block_size),
                    key);
            if (!ciphertext_block) {
                return nullptr;
            }

            for (unsigned int j = 0; j < ciphertext_block->size(); j++) {
                (*ciphertext)[i + j] = (*ciphertext_block)[j];
            }
        }

        return ciphertext;
    }

    std::shared_ptr<std::vector<unsigned char>> ecb_decrypt(
            const decryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key) {
        if (buffer.size() % profile.block_size != 0) {
            return nullptr;
        }

        auto plaintext = std::make_shared<std::vector<unsigned char>>(buffer.size());

        for (unsigned int i = 0; i < buffer.size(); i += profile.block_size) {
            const auto plaintext_block = profile.decrypt(
                    std::vector<unsigned char>(
                        buffer.begin() + i,
                        buffer.begin() + i + profile.block_size),
                    key);
            if (!plaintext_block) {
                return nullptr;
            }

            for (unsigned int j = 0; j < plaintext_block->size(); j++) {
                (*plaintext)[i + j] = (*plaintext_block)[j];
            }
        }

        const bool ok = profile.unpad(*plaintext, profile.block_size);
        if (!ok) {
            return nullptr;
        }

        return plaintext;
    }

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
