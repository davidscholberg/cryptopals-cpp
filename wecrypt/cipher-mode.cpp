#include <memory>
#include <vector>

#include "wecrypt/aes.hpp"
#include "wecrypt/cipher-mode.hpp"
#include "wecrypt/padding.hpp"
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
}
