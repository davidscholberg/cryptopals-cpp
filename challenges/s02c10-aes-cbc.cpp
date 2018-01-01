#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "challenges/s02c10-aes-cbc.hpp"
#include "utils/utils.hpp"
#include "wecrypt/cipher-mode.hpp"
#include "wecrypt/string-conversion.hpp"

const std::string s02::c10::challenge_arg("s02c10");

int s02::c10::aes_cbc(int argc, char **argv) {
    if (argc != 5) {
        std::cerr
            << "usage: "
            << argv[0] << " "
            << s02::c10::challenge_arg
            << " <ciphertext-file>"
            << " <key-file>"
            << " <iv-file>"
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

    auto iv_str = utils::get_file_contents(std::string(argv[4]));
    if (!iv_str) {
        std::cerr << "error: couldn't open iv file" << std::endl;
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

    auto iv_buffer = wecrypt::hex_to_binary(*iv_str);
    if (!iv_buffer) {
        std::cerr << "error: iv is invalid hex" << std::endl;
        return 2;
    }

    auto plaintext_buffer = wecrypt::cbc_decrypt(
            wecrypt::aes_pkcs7_decrypt,
            *ciphertext_buffer,
            key_buffer,
            *iv_buffer);
    if (!plaintext_buffer) {
        std::cerr << "error: couldn't decrypt ciphertext" << std::endl;
        return 2;
    }

    auto ciphertext_buffer_new = wecrypt::cbc_encrypt(
            wecrypt::aes_pkcs7_encrypt,
            *plaintext_buffer,
            key_buffer,
            *iv_buffer);
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
    std::cout << "iv:  " << *iv_str << std::endl;
    std::cout << "plaintext decrypted with AES CBC: " << std::endl << plaintext_str << std::endl;

    return 0;
}
