#include <memory>
#include <vector>

#include "wecrypt/padding.hpp"

namespace wecrypt {
    std::shared_ptr<std::vector<unsigned char>> pkcs7_pad(
            const std::vector<unsigned char> &buffer,
            const unsigned int block_size) {
        unsigned char padding_length = block_size - (buffer.size() % block_size);
        auto padded_buffer = std::make_shared<std::vector<unsigned char>>(
                buffer.size() + padding_length);

        for (unsigned int i = 0; i < buffer.size(); i++) {
            (*padded_buffer)[i] = buffer[i];
        }

        for (unsigned int i = buffer.size(); i < padded_buffer->size(); i++) {
            (*padded_buffer)[i] = padding_length;
        }

        return padded_buffer;
    }

    // returns false if padding is not valid
    bool pkcs7_unpad(
            std::vector<unsigned char> &buffer,
            const unsigned int block_size) {
        if (buffer.size() == 0 || buffer.size() % block_size != 0) {
            return false;
        }

        unsigned char padding_length = buffer[buffer.size() - 1];
        if (padding_length > block_size || padding_length == 0) {
            return false;
        }

        for (unsigned int i = 0; i < padding_length; i++) {
            if (buffer[buffer.size() - 1 - i] != padding_length) {
                return false;
            }
        }

        buffer.erase(buffer.end() - padding_length, buffer.end());

        return true;
    }
}
