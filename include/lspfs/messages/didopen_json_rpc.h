#ifndef RPC_DIDOPEN_MESSAGE_H
#define RPC_DIDOPEN_MESSAGE_H

#include <lspfs/json_rpc_helper.hpp>

struct JsonRpcAbstractMessage
{
  static constexpr char DEFAULT_VERSION[] = "2.0"; ///< Default version for all json rpc messages


};

class DidOpenJsonRpc : public JsonRpcAbstractMessage
{
public:
    

};

#endif // RPC_DIDOPEN_MESSAGE_H
