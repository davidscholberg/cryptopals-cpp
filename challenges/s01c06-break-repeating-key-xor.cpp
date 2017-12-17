#include <algorithm>
#include <iostream>
#include <string>

#include "challenges/s01c06-break-repeating-key-xor.hpp"
#include "utils/utils.hpp"
#include "wecrypt/wecrypt.hpp"

const std::string s01::c06::challenge_arg("s01c06");

int s01::c06::break_repeating_key_xor(int argc, char **argv) {
    if (argc != 3) {
        std::cerr
            << "usage: "
            << argv[0]
            << " "
            << s01::c06::challenge_arg
            << " <ciphertext-file>"
            << std::endl;
        return 1;
    }

    auto ciphertext = utils::get_file_contents(std::string(argv[2]));
    if (!ciphertext) {
        std::cerr << "error: couldn't open ciphertext file" << std::endl;
        return 2;
    }

    // remove newlines (using erase-remove idiom)
    ciphertext->erase(
            std::remove(ciphertext->begin(), ciphertext->end(), '\n'),
            ciphertext->end());

    auto ciphertext_bytes = wecrypt::base64_to_binary(*ciphertext);
    if (!ciphertext_bytes) {
        std::cerr << "error: ciphertext is invalid base64" << std::endl;
        return 2;
    }
    auto key = wecrypt::break_xor_repeating_key(*ciphertext_bytes);
    auto plaintext_bytes = wecrypt::xor_repeating_key(*ciphertext_bytes, *key);
    if (!plaintext_bytes) {
        std::cerr << "error: couldn't xor key to ciphertext" << std::endl;
        return 2;
    }

    std::string key_str(key->begin(), key->end());
    std::string plaintext_str(plaintext_bytes->begin(), plaintext_bytes->end());

    std::cout << "guessed key: " << key_str << std::endl;
    std::cout << "plaintext: " << std::endl << plaintext_str << std::endl;

    return 0;
}
