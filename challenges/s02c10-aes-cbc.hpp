#ifndef S02C10_AES_CBC_HPP_
#define S02C10_AES_CBC_HPP_

#include <string>

namespace s02 {
    namespace c10 {
        extern const std::string challenge_arg;
        int aes_cbc(int argc, char **argv);
    }
}

#endif // S02C10_AES_CBC_HPP_
