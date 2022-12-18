#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <map>

#include "Type_Manager.h"


typedef std::map<std::string, std::string> Str_Values;

#ifndef VARIABLE

	#define DECLARE_VARIABLE \
		public: \
			static std::string get_estimated_type(); \
			void assign_values(const Str_Values& _str_values) override

	#define INIT_FIELDS(variable_type, parent_type) \
		std::string variable_type::get_estimated_type() { return parent_type::get_estimated_type() + "/" + std::string(#variable_type); } \
		void variable_type::assign_values(const Str_Values& _str_values) { \
			parent_type::assign_values(_str_values); \
			m_type = m_type + "/" + std::string(#variable_type);

	#define ADD_FIELD(type, field_reference, field_name) { \
		Str_Values::const_iterator check = _str_values.find(field_name); \
		if(check != _str_values.cend())  \
			Type_Manager::parse(#type, check->second, (void*)(&field_reference)); \
		}

	#define FIELDS_END }

#endif


class Array_Variable;

class Variable_Base
{
protected:
	std::string m_name;
	std::string m_type;

public:
	static std::string get_estimated_type() { return ""; };
	const std::string& get_actual_type() { return m_type; }
	virtual void assign_values(const Str_Values& /*_str_values*/);

private:
	Array_Variable* m_parent = nullptr;

public:
	Variable_Base();
	virtual ~Variable_Base();

public:
	void set_name(const std::string& _name);
	const std::string& get_name() const;

public:
	void set_parent(Array_Variable* _parent);
	Array_Variable* get_parent();

};


template<typename T>
T* cast_variable(Variable_Base* _var)
{
	if(_var == nullptr)
		return nullptr;

	std::string T_type = T::get_estimated_type();
	std::string Var_type = _var->get_actual_type();

	if(Var_type.size() < T_type.size())
		return nullptr;

	if(Var_type.substr(0, T_type.size()) != T_type)
		return nullptr;

	return (T*)_var;
}


#endif // TYPE_H
