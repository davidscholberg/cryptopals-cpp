#include <iostream>
#include <string>
#include <vector>

#include "challenges/s01c08-detect-aes-ecb.hpp"
#include "utils/utils.hpp"
#include "wecrypt/aes.hpp"
#include "wecrypt/info.hpp"
#include "wecrypt/string-conversion.hpp"

const std::string s01::c08::challenge_arg("s01c08");

int s01::c08::detect_aes_ecb(int argc, char **argv) {
    if (argc != 3) {
        std::cerr
            << "usage: "
            << argv[0] << " "
            << s01::c08::challenge_arg
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

    int most_identical_blocks = 0;
    unsigned int best_index = 0;
    for (unsigned int i = 0; i < buffers.size(); i++) {
        int identical_blocks = wecrypt::count_identical_blocks(
                buffers[i],
                wecrypt::aes_block_size);
        if (identical_blocks > most_identical_blocks) {
            most_identical_blocks = identical_blocks;
            best_index = i;
        }
    }

    auto hex_str = wecrypt::binary_to_hex(buffers[best_index]);

    std::cout << "line number:       " << best_index + 1 << std::endl;
    std::cout << "identical blocks:  " << most_identical_blocks << std::endl;
    std::cout << "hex string:        " << *hex_str << std::endl;

    return 0;
}
