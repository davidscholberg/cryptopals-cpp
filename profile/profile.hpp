#ifndef PROFILE_HPP_
#define PROFILE_HPP_

#include <string>

namespace profile {
    struct profile {
        std::string email;
        unsigned int uid;
        std::string role;
        bool parse_error;
    };

    std::string encode(const profile &p);

    profile decode(const std::string &profile_str);

    profile profile_for(std::string email);
}

#endif // PROFILE_HPP_
