#include "client_capabilities.h"

// see https://microsoft.github.io/language-server-protocol/specifications/specification-current with ClientCapabilities for more info

ClientCapabilities::operator nlohmann::json() const
{
  return nlohmann::json::object();
}
