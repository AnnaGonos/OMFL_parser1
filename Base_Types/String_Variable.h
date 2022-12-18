#ifndef STRING_VARIABLE_H
#define STRING_VARIABLE_H

#include <string>

#include "Variable_Base.h"


class String_Variable : public Variable_Base
{
private:
	DECLARE_VARIABLE;

private:
	std::string m_value;

public:
	operator const std::string&() const;
	void operator=(const std::string& _value);

};


#endif // STRING_VARIABLE_H
