#include <lspfs/messages/initialize_request.h>

namespace lspfs
{

InitializeRequestMessageBuilder
InitializeRequestMessage::create()
{
  return InitializeRequestMessageBuilder();
}

std::string_view
InitializeRequestMessage::methodName() const
{
  return { InitializeRequestMessage::METHOD_NAME };
}

/**
 * See https://microsoft.github.io/language-server-protocol/specifications/specification-current/#initialize for details on this content
 * @return the serialized parameters
 */
void
InitializeRequestMessage::serializeParameters(nlohmann::json& content) const
{
    nlohmann::json& parameters = content[JsonRpcAbstractMessage::PARAMS_ATTRIBUTE];


    // first the process Id, mandatory field
    parameters["processId"] = (this->m_processId != 0) ? this->m_processId : nlohmann::json{};

    if ( this->m_clientInfo )
    {
      parameters["clientInfo"] = this->m_clientInfo;
    }

    parameters["rootUri"] = this->m_rootUri;

    parameters["capabilities"] = this->m_clientCapabilities; // we MUST have proper client capabilities
    
    parameters["trace"] =  this->m_trace;

    if( this->m_workspaceFolders.size())
    {
      parameters["workspaceFolders"] = this->m_workspaceFolders;
    }
}

std::ostream&
operator<<(std::ostream& os, const InitializeRequestMessage& obj)
{
  os << "InitializeRequestMessage\n";
  return os;
}

InitializeRequestMessageBuilder&
InitializeRequestMessageBuilder::withProcessid(size_t pid)
{
  this->m_msg.m_processId = pid;
  return *this;
}

InitializeRequestMessageBuilder&
InitializeRequestMessageBuilder::withClientInfo(ClientInfo info)
{
  this->m_msg.m_clientInfo = info;
  return *this;
}

InitializeRequestMessageBuilder&
InitializeRequestMessageBuilder::withRootUri(file_uri_t rootUri)
{
  this->m_msg.m_rootUri = std::move(rootUri);
  return *this;
}

InitializeRequestMessageBuilder&
InitializeRequestMessageBuilder::withCapabilities(ClientCapabilities caps)
{
  this->m_msg.m_clientCapabilities = std::move(caps);
  return *this;
}

InitializeRequestMessageBuilder&
InitializeRequestMessageBuilder::tracingLevel(TraceLevels level)
{
  switch (level)
  {
    case TraceLevels::OFF:
      this->m_msg.m_trace = "off";
      break;
    case TraceLevels::MESSAGES:
      this->m_msg.m_trace = "messages";
      break;

    case TraceLevels::VERBOSE:
      this->m_msg.m_trace = "verbose";
      break;
  }
  return *this;
}

InitializeRequestMessageBuilder::operator InitializeRequestMessage() const
{
  return std::move(this->m_msg);
}
}