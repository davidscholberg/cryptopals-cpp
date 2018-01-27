#ifndef USER_PROFILE_HPP_
#define USER_PROFILE_HPP_

#include <memory>
#include <string>

namespace user_profile {
    class user_profile {
        public:
            // throws std::invalid_argument on parse errors
            user_profile(const std::string &profile_str);
            user_profile(
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
    std::shared_ptr<user_profile> decode(const std::string &profile_str);

    // returns user_profile for the given email with uid 10 and role user.
    user_profile user_profile_for(const std::string &email);
}

#endif // USER_PROFILE_HPP_
