/**
 * Name: AfmSocketDefines.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_SOCKET_DEFINES
#define _H_AFM_SOCKET_DEFINES

#include <json.hpp>

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace afm {
    namespace communications {
        using SocketOptions = nlohmann::json;
        using SocketBuffer = std::vector<uint8_t>;

        const int32_t sc_closedSocket = -1;
        const std::string sc_socketURL = "url";
        const std::string sc_socketPort = "port";
    }
}
#endif