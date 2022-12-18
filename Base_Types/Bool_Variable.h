#ifndef BOOL_VARIABLE_H
#define BOOL_VARIABLE_H

#include "Variable_Base.h"


class Bool_Variable : public Variable_Base
{
private:
	DECLARE_VARIABLE;

private:
	bool m_value = false;

public:
	operator bool() const;
	void operator=(bool _value);

};


#endif // BOOL_VARIABLE_H
