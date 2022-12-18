#include "Int_Variable.h"


INIT_FIELDS(Int_Variable, Variable_Base)
ADD_FIELD(int, m_value, "value")
FIELDS_END


Int_Variable::operator int() const
{
	return m_value;
}

void Int_Variable::operator=(int _value)
{
	m_value = _value;
}
