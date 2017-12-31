#ifndef ORACLE_HPP_
#define ORACLE_HPP_

#include <memory>
#include <vector>

#include "wecrypt/cipher-mode.hpp"

namespace wecrypt {
    std::shared_ptr<std::vector<unsigned char>> ecb_cbc_encrypt_oracle(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer);

    // detects cipher mode used by ecb_cbc_encrypt_oracle
    cipher_mode attack_ecb_cbc_encrypt_oracle(
            const encryption_profile &profile);
}

#endif // ORACLE_HPP_
