#include <functional>
#include <iostream>
#include <unordered_map>

#include "challenges/s01c01-hex-to-base64.hpp"
#include "challenges/s01c02-fixed-xor.hpp"
#include "challenges/s01c03-single-byte-xor.hpp"
#include "challenges/s01c04-detect-single-byte-xor.hpp"
#include "challenges/s01c05-repeating-key-xor.hpp"
#include "challenges/s01c06-break-repeating-key-xor.hpp"
#include "challenges/s01c07-aes-ecb.hpp"
#include "challenges/s01c08-detect-aes-ecb.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " s<set-number>c<challenge-number> [args]" << std::endl;
        std::cerr << "e.g: " << argv[0] << " s01c01 deadbeef" << std::endl;
        return 1;
    }

    std::unordered_map<std::string, std::function<int(int, char**)>> function_map = {
        {s01::c01::challenge_arg, s01::c01::hex_to_base64},
        {s01::c02::challenge_arg, s01::c02::fixed_xor},
        {s01::c03::challenge_arg, s01::c03::single_byte_xor},
        {s01::c04::challenge_arg, s01::c04::detect_single_byte_xor},
        {s01::c05::challenge_arg, s01::c05::repeating_key_xor},
        {s01::c06::challenge_arg, s01::c06::break_repeating_key_xor},
        {s01::c07::challenge_arg, s01::c07::aes_ecb},
        {s01::c08::challenge_arg, s01::c08::detect_aes_ecb},
    };

    auto challenge_func = function_map.find(argv[1]);

    if (challenge_func == function_map.end()) {
        std::cerr << "error: couldn't find challenge " << argv[1] << std::endl;
        return 2;
    }

    return challenge_func->second(argc, argv);
}
