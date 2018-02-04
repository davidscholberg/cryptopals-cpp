#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#include "challenges/s02c13-ecb-cut-and-paste.hpp"
#include "utils/user-profile.hpp"
#include "wecrypt/cipher-mode.hpp"
#include "wecrypt/oracle.hpp"
#include "wecrypt/random.hpp"

const std::string s02::c13::challenge_arg("s02c13");

int s02::c13::ecb_cut_and_paste(int argc, char **argv) {
    if (argc != 2) {
        std::cerr
            << "usage: "
            << argv[0] << " "
            << s02::c13::challenge_arg
            << std::endl;
        return 1;
    }

    const std::vector<unsigned char> key(*wecrypt::get_random_bytes(16));

    std::string email_addr("l33t@h4x.guru");
    const auto encrypted_profile = wecrypt::user_profile_encrypt_oracle(
            wecrypt::aes_pkcs7_encrypt,
            key,
            email_addr);
    if (!encrypted_profile) {
        std::cerr << "error: couldn't get encrypted profile" << std::endl;
        return 2;
    }

    const std::string admin_str("admin");
    auto encrypted_admin = ecb_encrypt(
            wecrypt::aes_pkcs7_encrypt,
            std::vector<unsigned char>(admin_str.begin(), admin_str.end()),
            key);
    if (!encrypted_admin) {
        std::cerr << "error: couldn't encrypt admin string" << std::endl;
        return 2;
    }

    std::vector<unsigned char> cut_and_paste(
            encrypted_profile->begin(),
            encrypted_profile->begin() + 32);
    std::copy(
            encrypted_admin->begin(),
            encrypted_admin->end(),
            std::back_inserter(cut_and_paste));

    const auto decrypted_profile = wecrypt::user_profile_decrypt(
            wecrypt::aes_pkcs7_decrypt,
            key,
            cut_and_paste);
    if (!decrypted_profile) {
        std::cerr << "error: couldn't get decrypted profile" << std::endl;
        return 2;
    }

    std::cout << "email:           " << email_addr << std::endl;
    std::cout << "encoded profile: " << decrypted_profile->encode() << std::endl;

    return 0;
}
