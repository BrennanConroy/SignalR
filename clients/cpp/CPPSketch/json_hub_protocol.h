#pragma once
#ifndef _JSONHUBPROTOCOL_H
#define _JSONHUBPROTOCOL_H

#include <tuple>
#include <string>
#include <sstream>

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

#include "transfer_format.h"

namespace signalR
{
    template <typename T>
    rapidjson::Value to_json(T item, rapidjson::MemoryPoolAllocator<>& alloc);

    template <typename T>
    T from_json(const rapidjson::Value& item);

    class json_hub_protocol
    {
    public:
        template <typename ...T>
        std::tuple<T...> parse_message(const std::string& data) const
        {
            /*auto parsed = web::json::value::parse(utility::conversions::to_string_t(data));
            if (!parsed.is_array())
            {
                throw std::exception("expected json array");
            }
            if (parsed.size() != sizeof...(T))
            {
                throw std::exception("incorrect number of arguments");
            }*/
            auto d = rapidjson::Document();
            d.Parse(data.c_str());
            return parse_args<T...>(d["arguments"].GetArray());
        }

        template <typename ...T>
        std::string write_message(const T&... args) const
        {
            rapidjson::Document d;
            d.SetObject();
            auto& alloc = d.GetAllocator();

            rapidjson::Value arr(rapidjson::kArrayType);
            write_message_impl(arr, alloc, args...);
            d.AddMember("arguments", arr, alloc);

            rapidjson::StringBuffer buf;
            buf.Clear();
            rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
            d.Accept(writer);
            return buf.GetString();
        }

        static const transfer_format transferFormat = transfer_format::Text;
    private:
        template <typename First, typename ...T>
        void write_message_impl(rapidjson::Value& d, rapidjson::MemoryPoolAllocator<>& alloc, const First& first, const T&... args) const
        {
            auto k = to_json(first, alloc);
            d.PushBack(k, alloc);

            write_message_impl(d, alloc, args...);
        }

        template <typename T>
        void write_message_impl(rapidjson::Value& d, rapidjson::MemoryPoolAllocator<>& alloc, const T& arg) const
        {
            auto k = to_json(arg, alloc);
            d.PushBack(k, alloc);
        }

        template <typename T>
        std::tuple<T> parse_args(const rapidjson::Value::Array& arr) const
        {
            return std::make_tuple(from_json<T>(arr[0]));
        }

        template <typename T, typename T2>
        std::tuple<T, T2> parse_args(const rapidjson::Value::Array& arr) const
        {
            return std::make_tuple(from_json<T>(arr[0]), from_json<T2>(arr[1]));
        }

        template <typename T, typename T2, typename T3>
        std::tuple<T, T2, T3> parse_args(const rapidjson::Value::Array& arr) const
        {
            return std::make_tuple(from_json<T>(arr[0]), from_json<T2>(arr[1]), from_json<T3>(arr[2]));
        }
    };
}

#endif