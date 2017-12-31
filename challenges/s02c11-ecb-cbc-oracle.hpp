#ifndef S02C11_ECB_CBC_ORACLE_HPP_
#define S02C11_ECB_CBC_ORACLE_HPP_

#include <string>

namespace s02 {
    namespace c11 {
        extern const std::string challenge_arg;
        int ecb_cbc_oracle(int argc, char **argv);
    }
}

#endif // S02C11_ECB_CBC_ORACLE_HPP_
