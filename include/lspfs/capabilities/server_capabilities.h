#ifndef SERVER_CAPABILITIES_H
#define SERVER_CAPABILITIES_H

#include <nlohmann/json.hpp>

class ServerCapabilities
{
public:
  operator nlohmann::json() const;
};

#endif // SERVER_CAPABILITIES_H
