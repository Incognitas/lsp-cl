#ifndef INITIALIZE_REQUEST_MESSAGE_H
#define INITIALIZE_REQUEST_MESSAGE_H
#include <iosfwd>
#include <lspfs/json_rpc_helper.h>
#include <lspfs/messages/message_parameter.hpp>
#include <lspfs/capabilities/client_capabilities.h>


namespace lspfs
{


class InitializeRequestMessageBuilder;
class ClientInfo
{
public:
  constexpr operator bool () const { return false; } // for now

  operator nlohmann::json() const
  {
    return nlohmann::json(nlohmann::json{ { "name", nullptr }, { "version", nullptr } });
  }
};

 /**
 * @brief Represents the method: ‘textDocument/didOpen’ for JSONRPC
 *
 * Available params are the following:
 *
 * - textDocument : TextDocumentItem, represents the uri of the file to open
 *
 */
class InitializeRequestMessage : public JsonRpcRequestMessage
{
public:
  static constexpr const char METHOD_NAME[] = "initialize";

  friend class InitializeRequestMessageBuilder;
  friend std::ostream& operator<<(std::ostream& os, const InitializeRequestMessage& obj);
  static InitializeRequestMessageBuilder create();

  std::string_view methodName() const override;

private:
  
  InitializeRequestMessage() = default;

  size_t m_processId{ 0 };
  file_uri_t m_rootUri;
  std::string m_initializationOptions; // TODO: quels paramètres peut on passer ici ? dans la spec :
                                       // type "any"
  ClientCapabilities m_clientCapabilities;
  ClientInfo m_clientInfo; 
  std::string m_trace{ "off" }; // 'off' | 'messages' | 'verbose'
  using workspace_fodlers_list_t = std::vector<std::string>;
  workspace_fodlers_list_t m_workspaceFolders;

  void serializeParameters(nlohmann::json& content) const override;
};

class InitializeRequestMessageBuilder
{
public:
  enum class TraceLevels
  {
    OFF,
    MESSAGES,
    VERBOSE
  };

  InitializeRequestMessageBuilder& withProcessid(size_t pid);

  InitializeRequestMessageBuilder& withClientInfo(ClientInfo info);

  InitializeRequestMessageBuilder& withRootUri(file_uri_t rootUri);

  InitializeRequestMessageBuilder& withCapabilities(ClientCapabilities caps);

  InitializeRequestMessageBuilder& tracingLevel(TraceLevels level);

  operator InitializeRequestMessage() const;

  // InitializeRequestMessageBuilder& withWorkspaceFolders(std::vector<WorkspaceFolder> folders); // TODO later
private:
  InitializeRequestMessage m_msg;
  
};

}

#endif // INITIALIZE_REQUEST_MESSAGE_H