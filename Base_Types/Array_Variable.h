#ifndef ARRAY_VARIABLE_H
#define ARRAY_VARIABLE_H

#include <list>

#include "Variable_Base.h"


class Array_Variable : public Variable_Base
{
private:
	friend class Variable_Base;

private:
	DECLARE_VARIABLE;

private:
	std::list<Variable_Base*> m_childs;

public:
	void add_child(Variable_Base* _child);
	Variable_Base* remove_child(const std::string& _name);
	void remove_child(Variable_Base* _child);

	Variable_Base* get_child(const std::string& _name);
	std::list<Variable_Base*>& get_childs();

};


#endif // ARRAY_VARIABLE_H
