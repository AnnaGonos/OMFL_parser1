#ifndef INT_VARIABLE_H
#define INT_VARIABLE_H

#include "Variable_Base.h"


class Int_Variable : public Variable_Base
{
private:
	DECLARE_VARIABLE;

private:
	int m_value = 0;

public:
	operator int() const;
	void operator=(int _value);

};


#endif // INT_VARIABLE_H
