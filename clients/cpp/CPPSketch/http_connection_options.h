#pragma once

#include <string>

namespace signalR
{
    struct http_connection_options
    {
        int Transports;
        std::string Url;
    };
}