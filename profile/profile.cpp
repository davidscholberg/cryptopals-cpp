#include <algorithm>
#include <exception>
#include <sstream>
#include <string>
#include <list>

#include "profile/profile.hpp"

namespace profile {
    std::string encode(const profile &p) {
        std::stringstream encoded_profile;
        encoded_profile << "email=" << p.email;
        encoded_profile << "&uid=" << p.uid;
        encoded_profile << "&role=" << p.role;
        return encoded_profile.str();
    }

    profile decode(const std::string &profile_str) {
        // get list of fields
        std::stringstream profile_stream(profile_str);
        std::list<std::string> fields;
        for (std::string field; std::getline(profile_stream, field, '&'); ) {
            fields.push_back(field);
        }

        profile p = {};
        p.parse_error = true;

        for (auto field = fields.begin(); field != fields.end(); field++) {
            // get key val pair
            std::stringstream field_stream(*field);
            std::string key;
            if (!std::getline(field_stream, key, '=')) {
                return p;
            }
            std::string val;
            if (!std::getline(field_stream, val, '=')) {
                return p;
            }
            std::string bad;
            if (std::getline(field_stream, bad, '=')) {
                return p;
            }

            if (key == "email") {
                if (p.email.length() != 0) {
                    return p;
                }
                p.email = val;
            } else if (key == "uid") {
                if (p.uid != 0) {
                    return p;
                }
                try {
                    p.uid = std::stoi(val);
                } catch (std::exception& e) {
                    return p;
                }
            } else if (key == "role") {
                if (p.role.length() != 0) {
                    return p;
                }
                p.role = val;
            } else {
                return p;
            }
        }

        if (p.email.length() == 0 || p.uid == 0 || p.role.length() == 0) {
            return p;
        }

        p.parse_error = false;
        return p;
    }

    profile profile_for(std::string email) {
        // remove special chars (using erase-remove idiom)
        email.erase(
            std::remove_if(
                email.begin(),
                email.end(),
                [](char c){return c == '&' || c == '=';}),
            email.end());

        return {email, 10, "user", false};
    }
}
