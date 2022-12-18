#include "Bool_Variable.h"


INIT_FIELDS(Bool_Variable, Variable_Base)
ADD_FIELD(bool, m_value, "value")
FIELDS_END


Bool_Variable::operator bool() const
{
	return m_value;
}

void Bool_Variable::operator=(bool _value)
{
	m_value = _value;
}
