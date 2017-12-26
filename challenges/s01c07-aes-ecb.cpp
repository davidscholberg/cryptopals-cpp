#include <algorithm>
#include <memory>
#include <iostream>
#include <string>

#include "challenges/s01c07-aes-ecb.hpp"
#include "utils/utils.hpp"
#include "wecrypt/wecrypt.hpp"

const std::string s01::c07::challenge_arg("s01c07");

int s01::c07::aes_ecb(int argc, char **argv) {
    if (argc != 4) {
        std::cerr
            << "usage: "
            << argv[0] << " "
            << s01::c07::challenge_arg
            << " <ciphertext-file>"
            << " <key-file>"
            << std::endl;
        return 1;
    }
    auto ciphertext_str = utils::get_file_contents(std::string(argv[2]));
    if (!ciphertext_str) {
        std::cerr << "error: couldn't open ciphertext file" << std::endl;
        return 2;
    }

    auto key_str = utils::get_file_contents(std::string(argv[3]));
    if (!key_str) {
        std::cerr << "error: couldn't open key file" << std::endl;
        return 2;
    }

    // remove newlines (using erase-remove idiom)
    ciphertext_str->erase(
            std::remove(ciphertext_str->begin(), ciphertext_str->end(), '\n'),
            ciphertext_str->end());

    auto ciphertext_buffer = wecrypt::base64_to_binary(*ciphertext_str);
    if (!ciphertext_buffer) {
        std::cerr << "error: ciphertext is invalid base64" << std::endl;
        return 2;
    }

    std::vector<unsigned char> key_buffer(key_str->begin(), key_str->end());

    auto plaintext_buffer = wecrypt::ecb_decrypt(
            wecrypt::aes_pkcs7_decrypt,
            *ciphertext_buffer,
            key_buffer);
    if (!plaintext_buffer) {
        std::cerr << "error: couldn't decrypt ciphertext" << std::endl;
        return 2;
    }

    auto ciphertext_buffer_new = wecrypt::ecb_encrypt(
            wecrypt::aes_pkcs7_encrypt,
            *plaintext_buffer,
            key_buffer);
    if (!ciphertext_buffer_new) {
        std::cerr << "error: couldn't encrypt plaintext" << std::endl;
        return 2;
    }

    if (*ciphertext_buffer == *ciphertext_buffer_new) {
        std::cout << "samesies :D" << std::endl;
    } else {
        std::cerr << "diffsies D:" << std::endl;
        return 2;
    }

    std::string plaintext_str(plaintext_buffer->begin(), plaintext_buffer->end());

    std::cout << "key: " << *key_str << std::endl;
    std::cout << "plaintext decrypted with AES ECB: " << std::endl << plaintext_str << std::endl;

    return 0;
}
