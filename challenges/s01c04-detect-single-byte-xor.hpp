#ifndef S01C04_DETECT_SINGLE_BYTE_XOR_HPP_
#define S01C04_DETECT_SINGLE_BYTE_XOR_HPP_

#include <string>

namespace s01 {
    namespace c04 {
        extern const std::string challenge_arg;
        int detect_single_byte_xor(int argc, char **argv);
    }
}

#endif // S01C04_DETECT_SINGLE_BYTE_XOR_HPP_
