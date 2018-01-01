#include <iostream>
#include <string>
#include <vector>

#include "challenges/s02c09-pkcs7-padding.hpp"
#include "utils/utils.hpp"
#include "wecrypt/padding.hpp"

const std::string s02::c09::challenge_arg("s02c09");

int s02::c09::pkcs7_padding(int argc, char **argv) {
    if (argc != 4) {
        std::cerr
            << "usage: "
            << argv[0] << " "
            << s02::c09::challenge_arg
            << " <string-to-pad>"
            << " <block-size>"
            << std::endl;
        return 1;
    }

    std::string input_str(argv[2]);
    std::vector<unsigned char> input_buffer(input_str.begin(), input_str.end());

    int block_size = std::stoi(argv[3]);

    auto padded_buffer = wecrypt::pkcs7_pad(input_buffer, block_size);
    if (!padded_buffer) {
        std::cerr << "error: couldn't pad input" << std::endl;
        return 2;
    }

    auto hexdump_str = utils::hexdump(*padded_buffer);

    std::cout << "input string: " << input_str << std::endl;
    std::cout << "block size:   " << block_size << std::endl;
    std::cout << "hex dump of padded input: " << std::endl << *hexdump_str;

    return 0;
}
