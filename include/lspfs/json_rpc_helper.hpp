#ifndef JSON_RPC_HELPER_H
#define JSON_RPC_HELPER_H

#include <string>

template<typename T>
class JSONRPCHelper
{
public:
    using serialized_json_t = std::string;

    static inline serialized_json_t serialize(const T& src)
    {
      return T::serialize(src);
    }

    static inline T parseFromSerializedData(const serialized_json_t& data)
    {
      return T::deserialize(data);
    }
};

#endif // JSON_RPC_HELPER_H
