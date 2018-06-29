#pragma once
#ifndef _HUBCONNECTIONBUILDER_H
#define _HUBCONNECTIONBUILDER_H

#include <string>
#include <functional>
#include "json_hub_protocol.h"
#include "hub_connection.h"
#include "http_connection_factory.h"

namespace signalR
{
    namespace impl
    {
        template <typename C>
        struct has_parse_message
        {
            template <typename U>
            static char Test(decltype(&U::parse_message<>)*);
            template <typename U>
            static unsigned int Test(...);
            static const bool value = sizeof(Test<C>(0)) == sizeof(char);
        };

        template <typename C>
        struct has_write_message
        {
            template <typename U>
            static char Test(decltype(&U::write_message<>)*);
            template <typename U>
            static unsigned int Test(...);
            static const bool value = sizeof(Test<C>(0)) == sizeof(char);
        };

        template <typename P>
        class hub_connection_builder_impl;
    }

    class hub_connection_builder
    {
    public:
        hub_connection_builder() noexcept
        {
        }

        hub_connection_builder& configure_logging()
        {
            return *this;
        }

        hub_connection_builder& with_url(const std::string& url, std::function<void __cdecl(http_connection_options&)> configure = nullptr)
        {
            mOptions.Transports = 1;
            mOptions.Url = url;
            if (configure)
            {
                configure(mOptions);
            }

            return *this;
        }

        template <typename P>
        impl::hub_connection_builder_impl<P> use_protocol(P p)
        {
            return impl::hub_connection_builder_impl<P>(*this, p);
        }

        hub_connection<json_hub_protocol> build()
        {
            return std::move(hub_connection<json_hub_protocol>(mOptions, json_hub_protocol()));
        }

        template <typename Protocol>
        hub_connection<Protocol> build(Protocol protocol)
        {
            static_assert(impl::has_parse_message<Protocol>::value, "parse_message function expected from protocol");
            static_assert(impl::has_write_message<Protocol>::value, "write_message function expected from protocol");

            return hub_connection<Protocol>(mOptions, protocol);
        }

    private:
        http_connection_options mOptions;
    };

    namespace impl
    {
        template <typename Protocol>
        class hub_connection_builder_impl
        {
        public:
            hub_connection_builder_impl(hub_connection_builder& internalBuilder, Protocol protocol)
                : mBuilder(internalBuilder), mProtocol(protocol)
            {
            }

            hub_connection_builder_impl<Protocol>& configure_logging()
            {
                return *this;
            }

            hub_connection_builder_impl<Protocol>& with_url(const std::string& url, std::function<void __cdecl(http_connection_options&)> configure = nullptr)
            {
                mBuilder.with_url(url, configure);
                return *this;
            }

            hub_connection<Protocol> build()
            {
                static_assert(has_parse_message<Protocol>::value, "parse_message function expected from protocol");
                static_assert(has_write_message<Protocol>::value, "write_message function expected from protocol");
                return mBuilder.build<Protocol>(mProtocol);
            }
        private:
            hub_connection_builder & mBuilder;
            Protocol mProtocol;
        };
    }
}
#endif