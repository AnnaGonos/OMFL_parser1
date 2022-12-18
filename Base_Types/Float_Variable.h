#ifndef FLOAT_VARIABLE_H
#define FLOAT_VARIABLE_H

#include "Variable_Base.h"


class Float_Variable : public Variable_Base
{
private:
	DECLARE_VARIABLE;

private:
	float m_value = 0.0f;

public:
	operator float() const;
	void operator=(float _value);

};


#endif // FLOAT_VARIABLE_H
