#ifndef S01C08_DETECT_AES_ECB_HPP_
#define S01C08_DETECT_AES_ECB_HPP_

#include <string>

namespace s01 {
    namespace c08 {
        extern const std::string challenge_arg;
        int detect_aes_ecb(int argc, char **argv);
    }
}

#endif // S01C08_DETECT_AES_ECB_HPP_
