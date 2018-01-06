#ifndef S02C12_ECB_ORACLE_HPP_
#define S02C12_ECB_ORACLE_HPP_

#include <string>

namespace s02 {
    namespace c12 {
        extern const std::string challenge_arg;
        int ecb_oracle(int argc, char **argv);
    }
}

#endif // S02C12_ECB_ORACLE_HPP_
