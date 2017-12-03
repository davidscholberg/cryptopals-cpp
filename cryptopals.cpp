#include <iostream>

#include "cryptotools/cryptotools.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <hex-string>" << std::endl;
        return 1;
    }

    auto hex_str = std::make_shared<std::string>(argv[1]);
    auto base64_str = cryptotools::hex_to_base64(hex_str);

    if (!base64_str) {
        std::cerr << "error: couldn't convert arg to base64" << std::endl;
        return 2;
    }

    std::cout << "hex: " << *hex_str << std::endl;
    std::cout << "base64: " << *base64_str << std::endl;
}
