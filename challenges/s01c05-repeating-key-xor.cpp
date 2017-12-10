#include <iostream>
#include <string>

#include "challenges/s01c05-repeating-key-xor.hpp"
#include "utils/utils.hpp"
#include "wecrypt/wecrypt.hpp"

const std::string s01::c05::challenge_arg("s01c05");

int s01::c05::repeating_key_xor(int argc, char **argv) {
    if (argc != 4) {
        std::cerr
            << "usage: "
            << argv[0]
            << " "
            << s01::c05::challenge_arg
            << " <clear-text-file>"
            << " <key-file>"
            << std::endl;
        return 1;
    }

    auto clear_text = utils::get_file_contents(std::string(argv[2]));
    auto key_str = utils::get_file_contents(std::string(argv[3]));

    if (!clear_text || !key_str) {
        std::cerr << "error: couldn't open clear text or key file" << std::endl;
        return 2;
    }

    std::vector<unsigned char> clear_text_buffer(clear_text->begin(), clear_text->end());
    std::vector<unsigned char> key_buffer(key_str->begin(), key_str->end());

    auto xor_bytes = wecrypt::xor_repeating_key(clear_text_buffer, key_buffer);

    if (!xor_bytes) {
        std::cerr << "error: couldn't xor clear text with key" << std::endl;
        return 2;
    }

    auto xor_hex = wecrypt::binary_to_hex(*xor_bytes);

    std::cout << "clear text: " << *clear_text << std::endl;
    std::cout << "key: " << *key_str << std::endl;
    std::cout << "xor: " << *xor_hex << std::endl;

    return 0;
}
