#include <lspfs/capabilities/server_capabilities.h>

// see https://microsoft.github.io/language-server-protocol/specifications/specification-current with ServerCapabilities for more info

ServerCapabilities::operator nlohmann::json() const
{
  return nlohmann::json::object();
}
