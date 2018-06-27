#include <string>
#include <iostream>
#include <functional>
#include <future>

namespace signalR
{
    struct HttpConnectionOptions
    {
        // placeholders
        std::string Cookies;
        std::string Headers;
        std::string Certs;
    };

    enum LogLevel
    {
        Trace,
        Information,
        Warning,
        Error
    };

    template <typename Protocol>
    class hub_connection_builder_impl;

    class hub_connection_builder
    {
    public:
        hub_connection_builder();

        hub_connection_builder& configure_logging(LogLevel);

        hub_connection_builder& with_url(const std::string& url, std::function<void(HttpConnectionOptions&)> configure = nullptr);

        template <typename Protocol>
        _internal::hub_connection_builder_impl<Protocol> use_protocol(Protocol)
        {
            return _internal::hub_connection_builder_impl(*this, Protocol());
        }

        template <typename Protocol>
        hub_connection<Protocol> build(Protocol protocol);

        // default protocol if none specified
        hub_connection<JsonHubProtocol> build();
    };

    namespace _internal
    {
        template <typename Protocol>
        class hub_connection_builder_impl
        {
        public:
            hub_connection_builder_impl(hub_connection_builder& internalBuilder, Protocol protocol);

            hub_connection_builder_impl<Protocol>& configure_logging(LogLevel);

            hub_connection_builder_impl<Protocol>& with_url(const std::string& url, std::function<void(HttpConnectionOptions&)> configure = nullptr);

            hub_connection<Protocol> build();
        private:
            hub_connection_builder& mBuilder;
            Protocol mProtocol;
        };
    }

    template <typename Protocol>
    class hub_connection
    {
    public:
        explicit hub_connection(Protocol hubProtocol, const std::string url, const std::string& query_string = "");

        ~hub_connection();

        hub_connection(hub_connection &&rhs);

        hub_connection(const hub_connection&) = delete;

        hub_connection& operator=(const hub_connection&) = delete;

        std::future<void> start();

        std::future<void> stop();

        void on_closed(const std::function<void()>& closed_callback);

        template <typename ...T>
        void on(const std::string& name, const std::function<void(T...)>& methodHandler);

        template <typename R, typename ...T>
        std::future<R> invoke(const std::string& method_name, const T&... args);

        template <typename ...T>
        std::future<void> send(const std::string& method_name, const T&... args);
    };


    // Allows for users to define custom type serialization/deserialization, example below next to main
    template <typename T>
    rapidjson::Value to_json(T item, rapidjson::MemoryPoolAllocator<>& alloc);

    template <typename T>
    T from_json(const rapidjson::Value& item);
    class JsonHubProtocol
    {
    public:
        template <typename ...T>
        std::tuple<T...> parse_message(const std::string& data) const;

        template <typename ...T>
        std::string write_message(const T&... args);
    };
}

struct t
{
    bool b;
};

namespace signalR
{
    // add 'custom' type json parsing
    template <>
    t from_json<t>(const rapidjson::Value& item)
    {
        if (!item.IsBool())
        {
            throw std::runtime_error("Could not convert json to type 't'");
        }
        return t{ item.GetBool() };
    }

    template <>
    rapidjson::Value to_json<>(std::vector<int> item, rapidjson::MemoryPoolAllocator<>& alloc)
    {
        rapidjson::Value arr(rapidjson::kArrayType);

        for (auto i = 0; i < item.size(); ++i)
        {
            arr.PushBack(item[i], alloc);
        }

        return arr;
    }
}

int main(void)
{
    auto connection = signalR::hub_connection_builder()
        .use_protocol(signalR::JsonHubProtocol())
        .with_url("http://example.com", [](signalR::HttpConnectionOptions&) { /* � */ })
        .build();

    connection.on<int>("test", [](int i)
    {
        std::cout << i;
    });

    connection.on<int, std::string>("test2", [](int i, std::string s)
    {
        std::cout << i << " " << s;
    });

    connection.start().get();
    auto integer = connection.invoke<int>("echo", 10, "hello").get();
    connection.stop().get();
}