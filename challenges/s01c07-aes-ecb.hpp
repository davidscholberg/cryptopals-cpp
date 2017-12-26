#ifndef S01C07_AES_ECB_HPP_
#define S01C07_AES_ECB_HPP_

#include <string>

namespace s01 {
    namespace c07 {
        extern const std::string challenge_arg;
        int aes_ecb(int argc, char **argv);
    }
}

#endif // S01C07_AES_ECB_HPP_
