#include "json_hub_protocol.h"

namespace signalR
{
    template <>
    rapidjson::Value to_json<>(int32_t item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(int64_t item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(uint32_t item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(uint64_t item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(short item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(char item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(double item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(float item, rapidjson::MemoryPoolAllocator<>&)
    {
        return rapidjson::Value(item);
    }

    template <>
    rapidjson::Value to_json<>(std::string item, rapidjson::MemoryPoolAllocator<>& alloc)
    {
        return rapidjson::Value(item.c_str(), item.size(), alloc);
    }

    template <>
    rapidjson::Value to_json<>(const char* item, rapidjson::MemoryPoolAllocator<>& alloc)
    {
        return rapidjson::Value(item, alloc);
    }

    /*template <typename T>
    T from_json(const rapidjson::Value& item)
    {
        static_assert(false, "No conversion to json could be found for the given type, see below output for more info.");
    }*/

    template <>
    int from_json<int>(const rapidjson::Value& item)
    {
        if (!item.IsInt())
        {
            throw std::runtime_error("Could not convert json to type 'int'");
        }
        return item.GetInt();
    }

    template <>
    unsigned int from_json<unsigned int>(const rapidjson::Value& item)
    {
        if (!item.IsUint())
        {
            throw std::runtime_error("Could not convert json to type 'int'");
        }
        return item.GetUint();
    }

    template <>
    int64_t from_json<int64_t>(const rapidjson::Value& item)
    {
        if (!item.IsInt64())
        {
            throw std::runtime_error("Could not convert json to type 'int'");
        }
        return item.GetInt64();
    }

    template <>
    uint64_t from_json<uint64_t>(const rapidjson::Value& item)
    {
        if (!item.IsUint64())
        {
            throw std::runtime_error("Could not convert json to type 'int'");
        }
        return item.GetUint64();
    }

    template <>
    float from_json<float>(const rapidjson::Value& item)
    {
        if (!item.IsFloat())
        {
            throw std::runtime_error("Could not convert json to type 'float'");
        }
        return item.GetFloat();
    }

    template <>
    std::string from_json<std::string>(const rapidjson::Value& item)
    {
        if (!item.IsString())
        {
            throw std::runtime_error("Could not convert json to type 'std::string'");
        }
        return std::string(item.GetString(), item.GetStringLength());
    }

    template <>
    bool from_json<bool>(const rapidjson::Value& item)
    {
        if (!item.IsBool())
        {
            throw std::runtime_error("Could not convert json to type 'bool'");
        }
        return item.GetBool();
    }
}