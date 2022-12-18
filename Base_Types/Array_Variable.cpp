#include "Array_Variable.h"

INIT_FIELDS(Array_Variable, Variable_Base)
FIELDS_END;


void Array_Variable::add_child(Variable_Base *_child)
{
	_child->set_parent(this);
}

Variable_Base* Array_Variable::remove_child(const std::string &_name)
{
	std::list<Variable_Base*>::iterator it = m_childs.begin();
	while(it != m_childs.end())
	{
		if((*it)->get_name() == _name)
		{
			Variable_Base* ptr = *it;
			ptr->set_parent(nullptr);
			return ptr;
		}
		++it;
	}
	return nullptr;
}

void Array_Variable::remove_child(Variable_Base* _child)
{
	std::list<Variable_Base*>::iterator it = m_childs.begin();
	while(it != m_childs.end())
	{
		if(*it == _child)
		{
			Variable_Base* ptr = *it;
			ptr->set_parent(nullptr);
			return;
		}
		++it;
	}
}


Variable_Base* Array_Variable::get_child(const std::string& _name)
{
	std::list<Variable_Base*>::iterator it = m_childs.begin();
	while(it != m_childs.end())
	{
		if((*it)->get_name() == _name)
			return *it;
		++it;
	}
	return nullptr;
}

std::list<Variable_Base*>& Array_Variable::get_childs()
{
	return m_childs;
}
