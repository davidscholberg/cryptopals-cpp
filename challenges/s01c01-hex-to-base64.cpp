#include <iostream>
#include <string>

#include "challenges/s01c01-hex-to-base64.hpp"
#include "wecrypt/wecrypt.hpp"

const std::string s01::c01::challenge_arg("s01c01");

int s01::c01::hex_to_base64(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " " << s01::c01::challenge_arg << " <hex-string>" << std::endl;
        return 1;
    }

    std::string hex_str(argv[2]);
    auto base64_str = wecrypt::hex_to_base64(hex_str);

    if (!base64_str) {
        std::cerr << "error: couldn't convert arg to base64" << std::endl;
        return 2;
    }

    auto new_hex_str = wecrypt::base64_to_hex(*base64_str);

    if (!new_hex_str) {
        std::cerr << "error: couldn't convert base64 back to hex" << std::endl;
        return 2;
    }

    std::cout << "orig hex: " << hex_str << std::endl;
    std::cout << "base64: " << *base64_str << std::endl;
    std::cout << "new hex:  " << *new_hex_str << std::endl;

    if (hex_str != *new_hex_str) {
        std::cerr << "error: original and converted hex don't match" << std::endl;
        return 3;
    }

    return 0;
}
