/**
 * @brief JSONUtil
 * 
 * Utilities for JSON
 * 
 */

#ifndef _H_JSONUTILS
#define _H_JSONUTILS

#include <json.hpp>

namespace afm {
    namespace communications {
        template <typename T>
        bool extractValue(const nlohmann::json &values, const std::string &name, T &value)
        {
            bool success = false;
            nlohmann::json::const_iterator iter = values.find(name);
            if (iter != values.end()) {
                value = iter->get<T>();
                success = true;
            }
            return success;
        }
    }
}
#endif