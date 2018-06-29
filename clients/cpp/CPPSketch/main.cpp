#include <future>
#include <iostream>
#include <functional>
#include <map>
#include <sstream>

#include "hub_connection_builder.h"

namespace signalR
{
    // add 'custom' type json parsing
    template <>
    std::vector<int> from_json<std::vector<int>>(const rapidjson::Value& item)
    {
        if (!item.IsArray())
        {
            throw std::runtime_error("Could not convert json to type 'std::vector<int>'");
        }
        std::vector<int> vec;
        for (auto& i : item.GetArray())
        {
            if (i.IsInt())
            {
                vec.push_back(i.GetInt());
            }
        }
        return vec;
    }

    template <>
    rapidjson::Value to_json<>(std::vector<int> item, rapidjson::MemoryPoolAllocator<>& alloc)
    {
        rapidjson::Value arr(rapidjson::kArrayType);

        for (size_t i = 0; i < item.size(); ++i)
        {
            arr.PushBack(item[i], alloc);
        }

        return arr;
    }
}

auto doCon()
{
    auto connection = signalR::hub_connection_builder()
        .use_protocol(signalR::json_hub_protocol())
        .with_url("http://example.com", [](signalR::http_connection_options&) {})
        .configure_logging()
        .build();

    connection.on<float>("test", [](float i)
    {
        std::cout << "from test " << i << std::endl;
    });

    connection.on<int, std::string, std::vector<int>>("test2", [](int i, std::string s, std::vector<int> v)
    {
        std::cout << "from test2 " << i << " " << s.c_str();
        for (auto val : v)
        {
            std::cout << " " << val;
        }
        std::cout << std::endl;
    });

    connection.start().get();
    auto integer = connection.invoke<int>("", 10Ui64, 1.f, std::string("hello"), std::vector<int>{2, 5, 3}).get();
    connection.stop().get();

    return connection;
}

int main()
{
    auto c = doCon();
    c.mHandlers["test"]("{\"arguments\":[5.0]}");
    c.mHandlers["test2"]("{\"arguments\":[5, \"word\", [2,1,1]]}");

    char i;
    std::cin >> i;
}