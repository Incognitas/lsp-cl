#ifndef JSON_RPC_HELPER_H
#define JSON_RPC_HELPER_H

#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <string_view>

using file_uri_t = std::string;
using serialized_json_t = std::string;

enum class MessageType
{
  UNKNOWN = 0,
  NOTIFICATION = 1,
  REQUEST = 2,
  RESPONSE = 4
};

struct JsonRpcAbstractMessage
{
  virtual bool deserialize(const serialized_json_t& data) = 0;
  virtual serialized_json_t serialize() const = 0;
  virtual MessageType type() = 0;

  static constexpr char DEFAULT_JSON_RPC_VERSION[] =
    "2.0"; ///< Default version for all json rpc messages

  static constexpr char METHOD_NAME_ATTRIBUTE[] = "method";
  static constexpr char PARAMS_ATTRIBUTE[] = "params";
  static constexpr char ERROR_ATTRIBUTE[] = "error";
  static constexpr char ID_NAME_ATTRIBUTE[] = "id";

protected:
  static nlohmann::json bootstrapMessage();

  virtual std::string_view methodName() const = 0;
};

struct JsonRpcNotificationMessage : public JsonRpcAbstractMessage
{
  inline MessageType type() final override { return MessageType::NOTIFICATION; }

  bool deserialize(const serialized_json_t& data) override final;
  serialized_json_t serialize() const override final;

};

struct JsonRpcRequestMessage : public JsonRpcAbstractMessage
{
  inline MessageType type() final override { return MessageType::REQUEST; }

  bool deserialize(const serialized_json_t& data) override final;
  serialized_json_t serialize() const override final;

protected:
  /**
   * @brief Updates internal JsonRpcNotificationMessage::m_parameters structure with sub classes
   * content
   *
   * By default, this function does nothing
   *
   * This function is called each time JsonRpcNotificationMessage::serialize() is called
   *
   */
  virtual void serializeParameters(nlohmann::json& content) const = 0;

  static size_t ms_LatestIdOperation;
  
};

struct JsonRpcResponseMessage : public JsonRpcAbstractMessage
{
  inline MessageType type() final override { return MessageType::RESPONSE; }

  bool deserialize(const serialized_json_t& data) override final;
  serialized_json_t serialize() const override final;

  static constexpr char RESULT_ATTRIBUTE[] = "result";
};

#endif // JSON_RPC_HELPER_H
