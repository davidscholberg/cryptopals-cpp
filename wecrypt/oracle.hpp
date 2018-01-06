#ifndef ORACLE_HPP_
#define ORACLE_HPP_

#include <memory>
#include <vector>

#include "wecrypt/cipher-mode.hpp"

namespace wecrypt {
    std::shared_ptr<std::vector<unsigned char>> ecb_cbc_encrypt_oracle(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer);

    std::shared_ptr<std::vector<unsigned char>> ecb_encrypt_oracle(
            const encryption_profile &profile,
            const std::vector<unsigned char> &buffer);

    // detects cipher mode used by ecb_cbc_encrypt_oracle
    cipher_mode attack_ecb_cbc_encrypt_oracle(
            const encryption_profile &profile);

    // determines unknown string appended to plaintext in ecb_encrypt_oracle
    std::shared_ptr<std::vector<unsigned char>> attack_ecb_encrypt_oracle(
            const encryption_profile &profile);

    // determine byte at boundary block for ecb_encrypt_oracle
    bool break_ecb_boundary_block(
            const encryption_profile &profile,
            const std::vector<unsigned char> &plaintext_minus_one,
            const std::vector<unsigned char> &ciphertext,
            unsigned int block_size,
            unsigned char *byte);
}

#endif // ORACLE_HPP_
