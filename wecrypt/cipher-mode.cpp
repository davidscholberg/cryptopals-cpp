#include <algorithm>
#include <memory>
#include <vector>

#include "wecrypt/cipher-mode.hpp"

namespace wecrypt {
    std::shared_ptr<std::vector<unsigned char>> ecb_encrypt(
            encrypt_function encrypt,
            pad_function pad,
            const unsigned int block_size,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key) {
        auto padded_buffer = pad(buffer, block_size);

        auto ciphertext = std::make_shared<std::vector<unsigned char>>(padded_buffer->size());

        for (unsigned int i = 0; i < padded_buffer->size(); i += block_size) {
            auto ciphertext_block = encrypt(
                    std::vector<unsigned char>(
                        padded_buffer->begin() + i,
                        padded_buffer->begin() + i + block_size),
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
            decrypt_function decrypt,
            unpad_function unpad,
            const unsigned int block_size,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key) {
        if (buffer.size() % block_size != 0) {
            return nullptr;
        }

        auto plaintext = std::make_shared<std::vector<unsigned char>>(buffer.size());

        for (unsigned int i = 0; i < buffer.size(); i += block_size) {
            auto plaintext_block = decrypt(
                    std::vector<unsigned char>(
                        buffer.begin() + i,
                        buffer.begin() + i + block_size),
                    key);
            if (!plaintext_block) {
                return nullptr;
            }

            for (unsigned int j = 0; j < plaintext_block->size(); j++) {
                (*plaintext)[i + j] = (*plaintext_block)[j];
            }
        }

        bool ok = unpad(*plaintext, block_size);
        if (!ok) {
            return nullptr;
        }

        return plaintext;
    }
}
