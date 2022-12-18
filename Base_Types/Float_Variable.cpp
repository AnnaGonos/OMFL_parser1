#include "Float_Variable.h"


INIT_FIELDS(Float_Variable, Variable_Base)
ADD_FIELD(float, m_value, "value")
FIELDS_END


Float_Variable::operator float() const
{
	return m_value;
}

void Float_Variable::operator=(float _value)
{
	m_value = _value;
}
