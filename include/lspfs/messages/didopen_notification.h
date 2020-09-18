#ifndef DIDOPEN_NOTIFICATION_MESSAGE_H
#define DIDOPEN_NOTIFICATION_MESSAGE_H

#include <lspfs/json_rpc_helper.h>
#include <lspfs/messages/message_parameter.hpp>

namespace lspfs
{
/**
 * @brief Represents the method: ‘textDocument/didOpen’ for JSONRPC
 *
 * Available params are the following:
 *
 * - textDocument : TextDocumentItem, represents the uri of the file to open
 *
 */
class DidOpenNotificationMessage : public JsonRpcNotificationMessage
{
public:
  static constexpr const char METHOD_NAME[] = "textDocument/didOpen";
  /**
   * @brief Constructor
   *
   * \param documentPath
   */
  explicit DidOpenNotificationMessage(file_uri_t&& documentPath);

  std::string_view methodName() const override;
};

}

#endif // DIDOPEN_NOTIFICATION_MESSAGE_H
