#include "Variable_Base.h"
#include "Base_Types/Array_Variable.h"


void Variable_Base::assign_values(const Str_Values &)
{

}



Variable_Base::Variable_Base()
{

}

Variable_Base::~Variable_Base()
{

}



void Variable_Base::set_name(const std::string& _name)
{
	m_name = _name;
}

const std::string& Variable_Base::get_name() const
{
	return m_name;
}



void Variable_Base::set_parent(Array_Variable *_parent)
{
	if(m_parent == _parent)
		return;

	if(m_parent == nullptr)
	{
		m_parent = _parent;
		m_parent->m_childs.push_back(this);
		return;
	}

	std::list<Variable_Base*>::iterator it = m_parent->m_childs.begin();
	while(it != m_parent->m_childs.end())
	{
		if(*it == this)
		{
			m_parent->m_childs.erase(it);
			break;
		}
		++it;
	}

	m_parent = _parent;

	if(_parent != nullptr)
		m_parent->m_childs.push_back(this);
}

Array_Variable* Variable_Base::get_parent()
{
	return m_parent;
}
