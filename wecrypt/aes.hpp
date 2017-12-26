#ifndef AES_HPP_
#define AES_HPP_

#include <memory>
#include <vector>

namespace wecrypt {
    // size is in bytes
    extern const unsigned int aes_block_size;

    std::shared_ptr<std::vector<unsigned char>> aes_encrypt(
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    std::shared_ptr<std::vector<unsigned char>> aes_decrypt(
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);
}

#endif // AES_HPP_
