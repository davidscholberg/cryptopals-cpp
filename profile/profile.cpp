#include <algorithm>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>

#include "profile/profile.hpp"

namespace profile {
    // throws std::invalid_argument on parse errors
    profile::profile(const std::string &profile_str) {
        // get list of fields
        std::stringstream profile_stream(profile_str);
        std::list<std::string> fields;
        for (std::string field; std::getline(profile_stream, field, '&'); ) {
            fields.push_back(field);
        }

        for (auto field = fields.begin(); field != fields.end(); field++) {
            // get key val pair
            std::stringstream field_stream(*field);
            std::string key;
            if (!std::getline(field_stream, key, '=')) {
                throw std::invalid_argument("profile parse error");
            }
            std::string val;
            if (!std::getline(field_stream, val, '=')) {
                throw std::invalid_argument("profile parse error");
            }
            std::string bad;
            if (std::getline(field_stream, bad, '=')) {
                throw std::invalid_argument("profile parse error");
            }

            if (key == "email") {
                if (email.length() != 0) {
                    throw std::invalid_argument("profile parse error");
                }
                email = val;
            } else if (key == "uid") {
                if (uid != 0) {
                    throw std::invalid_argument("profile parse error");
                }
                try {
                    uid = std::stoi(val);
                } catch (std::exception& e) {
                    throw std::invalid_argument("profile parse error");
                }
            } else if (key == "role") {
                if (role.length() != 0) {
                    throw std::invalid_argument("profile parse error");
                }
                role = val;
            } else {
                throw std::invalid_argument("profile parse error");
            }
        }

        if (email.length() == 0 || uid == 0 || role.length() == 0) {
            throw std::invalid_argument("profile parse error");
        }
    }

    profile::profile(
            const std::string &email,
            unsigned int uid,
            const std::string &role) : email(email), uid(uid), role(role) {
        // remove special chars (using erase-remove idiom)
        this->email.erase(
            std::remove_if(
                this->email.begin(),
                this->email.end(),
                [](char c){return c == '&' || c == '=';}),
            this->email.end());
        this->role.erase(
            std::remove_if(
                this->role.begin(),
                this->role.end(),
                [](char c){return c == '&' || c == '=';}),
            this->role.end());
    }

    std::string profile::encode() const {
        std::stringstream encoded_profile;
        encoded_profile << "email=" << email;
        encoded_profile << "&uid=" << uid;
        encoded_profile << "&role=" << role;
        return encoded_profile.str();
    }

    // returns nullptr on parse error
    std::shared_ptr<profile> decode(const std::string &profile_str) {
        try {
            auto p = std::make_shared<profile>(profile_str);
            return p;
        } catch (std::invalid_argument &e) {
            return nullptr;
        }
    }

    // returns profile for the given email with uid 10 and role user.
    profile profile_for(const std::string &email) {
        return profile(email, 10, "user");
    }
}
