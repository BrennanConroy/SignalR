#pragma once

#include "transfer_format.h"
#include "http_connection_options.h"
#include <string>
#include <future>

namespace signalR
{
    namespace impl
    {
        class http_connection
        {
        public:
            http_connection() noexcept
                : mFormat(transfer_format::Text)
            {
            }

            http_connection(const http_connection_options& options) noexcept
                : mOptions(options), mFormat(transfer_format::Text), mUrl(options.Url)
            {
            }

            http_connection(http_connection&& rhs) noexcept
                : mOptions(rhs.mOptions), mFormat(rhs.mFormat), mUrl(std::forward<std::string>(rhs.mUrl))
            {
            }

            http_connection& operator=(http_connection&& rhs) noexcept
            {
                mOptions = rhs.mOptions;
                mFormat = rhs.mFormat;
                mUrl = std::forward<std::string>(rhs.mUrl);
                return *this;
            }

            http_connection(const http_connection&) = delete;
            http_connection& operator=(const http_connection&) = delete;

            std::future<void> start(transfer_format format)
            {
                mFormat = format;
                return std::async(std::launch::async, [] {});
            }

            std::future<void> dispose()
            {
                return std::async(std::launch::deferred, [] {});
            }
        private:
            transfer_format mFormat;
            http_connection_options mOptions;
            std::string mUrl;
        };
    }
}