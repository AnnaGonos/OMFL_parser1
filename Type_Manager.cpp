#include "Type_Manager.h"


std::map<std::string, Type_Utility> Type_Manager::m_registred_types;



void Type_Manager::register_type(const std::string &_type_name, const Type_Utility &_utility)
{
	regtypes_t::iterator check = m_registred_types.find(_type_name);
	if(check != m_registred_types.end())
		return;

	m_registred_types.emplace(_type_name, _utility);
}

void Type_Manager::parse(const std::string& _type_name, const std::string& _value_as_string, void* _allocate_to)
{
	regtypes_t::iterator utility_it = m_registred_types.find(_type_name);
	if(utility_it == m_registred_types.end())
		return;

	const Type_Utility& utility = utility_it->second;

	utility.parse_func(_allocate_to, _value_as_string);
}

void Type_Manager::free(const std::string& _type_name, void* _data_vptr)
{
	regtypes_t::iterator utility_it = m_registred_types.find(_type_name);
	if(utility_it == m_registred_types.end())
		return;

	const Type_Utility& utility = utility_it->second;

	utility.free_func(_data_vptr);
}

void Type_Manager::assign(const std::string& _type_name, void* _data_vptr, void* _assign_to)
{
	regtypes_t::iterator utility_it = m_registred_types.find(_type_name);
	if(utility_it == m_registred_types.end())
		return;

	const Type_Utility& utility = utility_it->second;

	utility.assign_func(_assign_to, _data_vptr);
}
