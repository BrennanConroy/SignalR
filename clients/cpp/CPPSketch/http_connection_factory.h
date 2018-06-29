#pragma once

#include <future>
#include <string>
#include "http_connection.h"
#include "transfer_format.h"

namespace signalR
{
    namespace impl
    {
        class http_connection_factory
        {
        public:
            http_connection_factory(const http_connection_options& options) noexcept
                : mOptions(options)
            {
            }

            std::future<http_connection> connect(transfer_format format)
            {
                return std::async(std::launch::async, [this, format]
                {
                    auto connection = http_connection(mOptions);
                    try
                    {
                        connection.start(format).get();
                    }
                    catch (...)
                    {
                        connection.dispose().get();
                        throw;
                    }

                    return connection;
                });
            }
        private:
            http_connection_options mOptions;
        };
    }
}