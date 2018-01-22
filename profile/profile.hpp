#ifndef PROFILE_HPP_
#define PROFILE_HPP_

#include <memory>
#include <string>

namespace profile {
    class profile {
        public:
            // throws std::invalid_argument on parse errors
            profile(const std::string &profile_str);
            profile(
                    const std::string &email,
                    unsigned int uid,
                    const std::string &role);
            std::string encode() const;
        private:
            std::string email;
            unsigned int uid;
            std::string role;
    };

    // returns nullptr on parse error
    std::shared_ptr<profile> decode(const std::string &profile_str);

    // returns profile for the given email with uid 10 and role user.
    profile profile_for(const std::string &email);
}

#endif // PROFILE_HPP_
