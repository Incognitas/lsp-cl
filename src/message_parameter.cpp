#include <lspfs/messages/message_parameter.hpp>

MessageParameter::MessageParameter(const parameter_name_t& name, const nlohmann::json& content)
  : m_name(name)
  , m_value{ content }
{
}

const MessageParameter::parameter_name_t&
MessageParameter::name() const
{
  return this->m_name;
}

bool
MessageParameter::isConstructed() const
{
  return this->m_value.is_structured();
}

nlohmann::json
MessageParameter::toJson() const
{
  return this->m_value;
}