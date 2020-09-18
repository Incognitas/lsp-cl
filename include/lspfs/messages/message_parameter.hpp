#ifndef RPC_MESSAGE_PARAMETER_H
#define RPC_MESSAGE_PARAMETER_H

#include <nlohmann/json.hpp>
#include <string>

class MessageParameter
{
public:
  using parameter_name_t = std::string;
  using serialized_message_content_t = std::string;

  MessageParameter(const parameter_name_t& name, const nlohmann::json& content);

  template<typename T>
  T value() const
  {
    return this->m_value.get<T>(m_value);
  }

  template<typename T>
  T value(const std::string& subPath)
  {
    return this->m_value.value(subPath, T{});
  }

  const parameter_name_t& name() const;

  /**
   * @brief Indicates whether . or not the given parameter contains sub tags
   *
   * \return true if it is the case, false otherwise
   */
  bool isConstructed() const;

  nlohmann::json toJson() const;

private:
  nlohmann::json m_value;
  parameter_name_t m_name;
};

#endif // RPC_MESSAGE_PARAMETER_H
