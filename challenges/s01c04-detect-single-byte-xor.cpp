#include <iostream>
#include <string>
#include <vector>

#include "challenges/s01c04-detect-single-byte-xor.hpp"
#include "utils/utils.hpp"
#include "wecrypt/string-conversion.hpp"
#include "wecrypt/xor.hpp"

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

    auto file_lines = utils::get_lines(std::string(argv[2]));

    if (!file_lines) {
        std::cerr << "error: couldn't open file" << std::endl;
        return 2;
    }

    std::vector<std::vector<unsigned char>> buffers(file_lines->size());

    for (int i = buffers.size() - 1; i >=0; i--) {
        auto bytes = wecrypt::hex_to_binary((*file_lines)[i]);
        if (!bytes) {
            std::cerr << "error: invalid hex in file" << std::endl;
            return 2;
        }
        buffers[i] = *bytes;
        file_lines->pop_back();
    }

    auto scores = wecrypt::detect_xor_single_byte(buffers);

    auto winning_score = (*scores)[0];
    unsigned char key = (*winning_score.scores)[0].byte;
    float score = (*winning_score.scores)[0].score;
    auto hex_bytes = buffers[winning_score.index];
    auto xor_bytes = wecrypt::xor_single_byte(hex_bytes, key);
    auto hex_str = wecrypt::binary_to_hex(hex_bytes);
    std::string xor_str(xor_bytes->begin(), xor_bytes->end());
    auto key_str = wecrypt::binary_to_hex(key);

    std::cout << "top result: " << std::endl;
    std::cout << "line number: " << winning_score.index + 1 << std::endl;
    std::cout << "hex string: " << hex_str << std::endl;
    std::cout << "key: 0x" << *key_str << " score: " << score << std::endl;
    std::cout << "xor message: " << xor_str << std::endl;

    return 0;
}
