#include <iostream>
#include <string>

#include "challenges/s02c12-ecb-oracle.hpp"
#include "wecrypt/cipher-mode.hpp"
#include "wecrypt/oracle.hpp"

const std::string s02::c12::challenge_arg("s02c12");

int s02::c12::ecb_oracle(int argc, char **argv) {
    if (argc != 2) {
        std::cerr
            << "usage: "
            << argv[0] << " "
            << s02::c12::challenge_arg
            << std::endl;
        return 1;
    }

    auto unknown_buffer = wecrypt::attack_ecb_encrypt_oracle(
            wecrypt::aes_pkcs7_encrypt);

    if (!unknown_buffer) {
        std::cerr << "error: couldn't determine unknown string" << std::endl;
        return 2;
    }

    std::string unknown_str(unknown_buffer->begin(), unknown_buffer->end());
    std::cout << "unknown string:" << std::endl << unknown_str;

    return 0;
}
