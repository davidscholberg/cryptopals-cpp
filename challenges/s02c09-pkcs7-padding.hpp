#ifndef S02C09_PKCS7_PADDING_HPP_
#define S02C09_PKCS7_PADDING_HPP_

#include <string>

namespace s02 {
    namespace c09 {
        extern const std::string challenge_arg;
        int pkcs7_padding(int argc, char **argv);
    }
}

#endif // S02C09_PKCS7_PADDING_HPP_
