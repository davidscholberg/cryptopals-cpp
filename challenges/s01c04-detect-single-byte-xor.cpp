#include <iostream>
#include <string>

#include "challenges/s01c04-detect-single-byte-xor.hpp"
#include "utils/utils.hpp"
#include "wecrypt/wecrypt.hpp"

const std::string s01::c04::challenge_arg("s01c04");

int s01::c04::detect_single_byte_xor(int argc, char **argv) {
    if (argc != 3) {
        std::cerr
            << "usage: "
            << argv[0]
            << " "
            << s01::c04::challenge_arg
            << " <filename>"
            << std::endl;
        return 1;
    }

    auto filename = std::make_shared<std::string>(argv[2]);

    auto file_lines = utils::get_lines(filename);

    if (!file_lines) {
        std::cerr << "error: couldn't open file" << std::endl;
        return 2;
    }

    auto scores = wecrypt::detect_xor_single_byte(file_lines);

    if (!scores) {
        std::cerr << "error: couldn't get xor scores" << std::endl;
        return 2;
    }

    auto winning_score = (*scores)[0];
    unsigned char key = (*winning_score.scores)[0].byte;
    float score = (*winning_score.scores)[0].score;
    auto hex_str = std::make_shared<std::string>((*file_lines)[winning_score.index]);
    auto hex_bytes = wecrypt::hex_to_binary(hex_str);
    auto xor_bytes = wecrypt::xor_single_byte(hex_bytes, key);
    std::string xor_str(xor_bytes->begin(), xor_bytes->end());
    auto key_str = wecrypt::binary_to_hex(key);

    std::cout << "top result: " << std::endl;
    std::cout << "line number: " << winning_score.index + 1 << std::endl;
    std::cout << "hex string: " << *hex_str << std::endl;
    std::cout << "key: 0x" << *key_str << " score: " << score << std::endl;
    std::cout << "xor message: " << xor_str << std::endl;

    return 0;
}
