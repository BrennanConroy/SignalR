#pragma once
#ifndef _HUBCONNECTION_H
#define _HUBCONNECTION_H

#include <map>
#include <future>
#include "http_connection_factory.h"

namespace signalR
{
    template <typename Protocol>
    class hub_connection
    {
    public:
        explicit hub_connection(impl::http_connection_factory factory, Protocol hubProtocol) noexcept
            : mProtocol(hubProtocol), mFactory(factory)
        {}

        ~hub_connection() {}

        hub_connection(hub_connection &&rhs) noexcept
            : mHandlers(std::move(rhs.mHandlers)), mProtocol(std::move(rhs.mProtocol)), mFactory(std::move(rhs.mFactory)), mConnection(std::move(rhs.mConnection))
        {
        }

        hub_connection(const hub_connection&) = delete;

        hub_connection& operator=(const hub_connection&) = delete;

        std::future<void> __cdecl start()
        {
            mConnection = mFactory.connect(mProtocol.transferFormat).get();

            return some_func();
        }

        std::future<void> __cdecl stop()
        {
            return some_func();
        }

        void __cdecl on_closed(const std::function<void __cdecl()>& closed_callback);

        template <typename ...T>
        void __cdecl on(const std::string& name, const std::function<void __cdecl (T...)>& methodHandler)
        {
            mHandlers[name] = [methodHandler, this](const std::string& arguments)
            {
                auto tuple = mProtocol.parse_message<T...>(arguments);
                invoke_with_args<T...>(methodHandler, tuple);
            };
        }

        template <typename R, typename ...T>
        std::future<R> invoke(const std::string& method_name, const T&... args)
        {
            auto msg = mProtocol.write_message<T...>(args...);
            std::cout << "json created: " << msg << std::endl;
            return std::async(std::launch::deferred, []() { return R(); });
        }

        template <typename ...T>
        std::future<void> send(const std::string& method_name, const T&... args)
        {
            auto msg = mProtocol.write_message<T...>(args...);
            return std::async(std::launch::deferred, []() { });
        }

        //private:
        std::future<void> some_func()
        {
            return std::async(std::launch::deferred, []() { std::cout << std::endl; });
        }

        std::map<std::string, std::function<void(const std::string&)>> mHandlers;
        Protocol mProtocol;
        impl::http_connection_factory mFactory;
        impl::http_connection mConnection;

    private:
        template <typename T>
        void invoke_with_args(const std::function<void(T)>& func, const std::tuple<T>& args)
        {
            func(std::get<0>(args));
        }

        template <typename T, typename T2>
        void invoke_with_args(const std::function<void(T, T2)>& func, const std::tuple<T, T2>& args)
        {
            func(std::get<0>(args), std::get<1>(args));
        }

        template <typename T, typename T2, typename T3>
        void invoke_with_args(const std::function<void(T, T2, T3)>& func, const std::tuple<T, T2, T3>& args)
        {
            func(std::get<0>(args), std::get<1>(args), std::get<2>(args));
        }

        template <typename T, typename T2, typename T3, typename T4>
        void invoke_with_args(const std::function<void(T, T2, T3, T4)>& func, const std::tuple<T, T2, T3, T4>& args)
        {
            func(std::get<0>(args), std::get<1>(args), std::get<2>(args), std::get<3>(args));
        }

        template <typename T, typename T2, typename T3, typename T4, typename T5>
        void invoke_with_args(const std::function<void(T, T2, T3, T4, T5)>& func, const std::tuple<T, T2, T3, T4, T5>& args)
        {
            func(std::get<0>(args), std::get<1>(args), std::get<2>(args), std::get<3>(args), std::get<4>(args));
        }

        template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
        void invoke_with_args(const std::function<void(T, T2, T3, T4, T5, T6)>& func, const std::tuple<T, T2, T3, T4, T5, T6>& args)
        {
            func(std::get<0>(args), std::get<1>(args), std::get<2>(args), std::get<3>(args), std::get<4>(args), std::get<5>(args));
        }

        template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
        void invoke_with_args(const std::function<void(T, T2, T3, T4, T5, T6, T7)>& func, const std::tuple<T, T2, T3, T4, T5, T6, T7>& args)
        {
            func(std::get<0>(args), std::get<1>(args), std::get<2>(args), std::get<3>(args), std::get<4>(args), std::get<5>(args), std::get<6>(args));
        }
    };
}
#endif