#include <lspfs/json_rpc_helper.h>

size_t JsonRpcRequestMessage::ms_LatestIdOperation = 1;


nlohmann::json
JsonRpcAbstractMessage::bootstrapMessage()
{
  nlohmann::json returnValue;
  returnValue["jsonrpc"] = DEFAULT_JSON_RPC_VERSION;
  return returnValue;
}

bool
JsonRpcNotificationMessage::deserialize(const serialized_json_t& data)
{
    // we don't want to deserialize for now as we are the client and don't need that
    // But I think it must be implemented later on so ...
    // TODO : implement this
  return false;
}

serialized_json_t
JsonRpcNotificationMessage::serialize() const
{
  nlohmann::json finalValue{ JsonRpcAbstractMessage::bootstrapMessage() };
  finalValue[JsonRpcAbstractMessage::METHOD_NAME_ATTRIBUTE] = this->methodName();
  

  return finalValue.dump();
}

bool
JsonRpcRequestMessage::deserialize(const serialized_json_t& data)
{
  return false;
}

serialized_json_t
JsonRpcRequestMessage::serialize() const
{
  nlohmann::json finalValue{ JsonRpcAbstractMessage::bootstrapMessage() };
  finalValue[JsonRpcAbstractMessage::METHOD_NAME_ATTRIBUTE] = this->methodName();
  finalValue[JsonRpcAbstractMessage::ID_NAME_ATTRIBUTE] = JsonRpcRequestMessage::ms_LatestIdOperation ++;
  this->serializeParameters(finalValue);

  return finalValue.dump();
}

