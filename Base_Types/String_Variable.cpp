#include "String_Variable.h"


INIT_FIELDS(String_Variable, Variable_Base)
ADD_FIELD(std::string, m_value, "value")
FIELDS_END


String_Variable::operator const std::string&() const
{
	return m_value;
}

void String_Variable::operator=(const std::string& _value)
{
	m_value = _value;
}
