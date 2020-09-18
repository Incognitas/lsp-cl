#ifndef CLIENT_CAPABILITIES_H
#define CLIENT_CAPABILITIES_H

#include <nlohmann/json.hpp>

class ClientCapabilities
{
public:
  operator nlohmann::json() const;
};

#endif // CLIENT_CAPABILITIES_H
