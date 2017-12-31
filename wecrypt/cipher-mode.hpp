#ifndef CIPHER_MODE_HPP_
#define CIPHER_MODE_HPP_

#include <functional>
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

    struct encryption_profile {
        std::function<encrypt_function> encrypt;
        std::function<pad_function> pad;
        unsigned int block_size;
    };

    struct decryption_profile {
        std::function<decrypt_function> decrypt;
        std::function<unpad_function> unpad;
        unsigned int block_size;
    };

    extern const encryption_profile aes_pkcs7_encrypt;
    extern const decryption_profile aes_pkcs7_decrypt;

    std::shared_ptr<std::vector<unsigned char>> cbc_encrypt(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key,
            const std::vector<unsigned char> &iv);

    std::shared_ptr<std::vector<unsigned char>> cbc_decrypt(
            const decryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key,
            const std::vector<unsigned char> &iv);

    std::shared_ptr<std::vector<unsigned char>> ecb_encrypt(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    std::shared_ptr<std::vector<unsigned char>> ecb_decrypt(
            const decryption_profile &profile,
            const std::vector<unsigned char> &buffer,
            const std::vector<unsigned char> &key);

    std::shared_ptr<std::vector<unsigned char>> random_ecb_cbc_encrypt(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer);

    enum cipher_mode {
        error,
        cbc,
        ecb
    };

    cipher_mode random_ecb_cbc_encrypt_oracle(
            const encryption_profile &profile);
}

#endif // CIPHER_MODE_HPP_
