#include <iostream>
#include <string>

#include "challenges/s01c03-single-byte-xor.hpp"
#include "wecrypt/string-conversion.hpp"
#include "wecrypt/xor.hpp"

const std::string s01::c03::challenge_arg("s01c03");

int s01::c03::single_byte_xor(int argc, char **argv) {
    if (argc != 3) {
        std::cerr
            << "usage: "
            << argv[0]
            << " "
            << s01::c03::challenge_arg
            << " <hex-string>"
            << std::endl;
        return 1;
    }

    std::string hex_str(argv[2]);

    auto hex_bytes = wecrypt::hex_to_binary(hex_str);

    if (!hex_bytes) {
        std::cerr << "error: invalid hex in args" << std::endl;
        return 2;
    }

    auto xor_scores = wecrypt::break_xor_single_byte(*hex_bytes);

    std::cout << "hex input: " << hex_str << std::endl;

    std::cout << "top result: " << std::endl;

    auto key_str = wecrypt::binary_to_hex((*xor_scores)[0].byte);
    auto xor_bytes = wecrypt::xor_single_byte(*hex_bytes, (*xor_scores)[0].byte);
    std::string xor_str(xor_bytes->begin(), xor_bytes->end());

    std::cout << "key: 0x" << *key_str << " score: " << (*xor_scores)[0].score << std::endl;
    std::cout << "xor message: " << xor_str << std::endl;

    return 0;
}
