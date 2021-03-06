#include <iostream>
#include <string>

#include "challenges/s01c02-fixed-xor.hpp"
#include "wecrypt/string-conversion.hpp"
#include "wecrypt/xor.hpp"

const std::string s01::c02::challenge_arg("s01c02");

int s01::c02::fixed_xor(int argc, char **argv) {
    if (argc != 4) {
        std::cerr
            << "usage: "
            << argv[0]
            << " "
            << s01::c02::challenge_arg
            << " <hex-string> <hex-string>"
            << std::endl;
        return 1;
    }

    std::string hex_str_a(argv[2]);
    std::string hex_str_b(argv[3]);

    auto hex_bytes_a = wecrypt::hex_to_binary(hex_str_a);
    auto hex_bytes_b = wecrypt::hex_to_binary(hex_str_b);

    if (!hex_bytes_a || !hex_bytes_b) {
        std::cerr << "error: invalid hex in args" << std::endl;
        return 2;
    }

    auto xor_bytes = wecrypt::xor_bytes(*hex_bytes_a, *hex_bytes_b);

    if (!xor_bytes) {
        std::cerr << "error: couldn't xor args" << std::endl;
        return 2;
    }

    auto xor_str = wecrypt::binary_to_hex(*xor_bytes);

    std::cout << "hex string a:   " << hex_str_a << std::endl;
    std::cout << "hex string b:   " << hex_str_b << std::endl;
    std::cout << "xor of a and b: " << *xor_str << std::endl;

    return 0;
}
