#include <lspfs/messages/didopen_notification.h>

namespace lspfs
{

DidOpenNotificationMessage::DidOpenNotificationMessage(file_uri_t&& documentPath)
{
  /*this->m_parameters.emplace_back(
    MessageParameter{ "textDocument", nlohmann::json::basic_json({ "uri", documentPath }) });*/
}

std::string_view
DidOpenNotificationMessage::methodName() const
{
  return DidOpenNotificationMessage::METHOD_NAME;
}

}