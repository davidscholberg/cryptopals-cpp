#ifndef CIPHER_MODE_HPP_
#define CIPHER_MODE_HPP_

#include <memory>
#include <vector>

namespace wecrypt {
    typedef std::shared_ptr<std::vector<unsigned char>> encrypt_function(
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    typedef std::shared_ptr<std::vector<unsigned char>> decrypt_function(
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    typedef std::shared_ptr<std::vector<unsigned char>> pad_function(
            const std::vector<unsigned char> &buffer,
            const unsigned int block_size);

    typedef bool unpad_function(
            std::vector<unsigned char> &buffer,
            const unsigned int block_size);

    std::shared_ptr<std::vector<unsigned char>> ecb_encrypt(
            encrypt_function encrypt,
            pad_function pad,
            const unsigned int block_size,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    std::shared_ptr<std::vector<unsigned char>> ecb_decrypt(
            decrypt_function decrypt,
            unpad_function unpad,
            const unsigned int block_size,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);
}

#endif // CIPHER_MODE_HPP_
