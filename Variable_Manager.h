#ifndef VARIABLE_MANAGER_H
#define VARIABLE_MANAGER_H

#include <list>
#include <map>

#include "OMFL_Reader.h"

#include "Base_Types/Variable_Base.h"
#include "Base_Types/Int_Variable.h"
#include "Base_Types/Bool_Variable.h"
#include "Base_Types/Float_Variable.h"
#include "Base_Types/String_Variable.h"
#include "Base_Types/Array_Variable.h"



class Variable_Manager
{
private:
	enum class Type
	{
		Unknown,
		Int,
		Float,
		Bool,
		String,
		Array
	};

private:
	std::list<Variable_Base*> m_variables;

	std::map<std::string, Variable_Manager> m_subsections;

public:
	Variable_Manager();
	~Variable_Manager();

private:
	Type M_get_type(const std::string& _value) const;
	std::string M_get_next_subsection_name(const std::string& _sections_path) const;
	Variable_Manager* M_get_subsection(const std::string& _sections_path, bool _create_if_not_found = false);

public:
	Variable_Base* add_variable(const Variable_Stub& _stub, Array_Variable* _parent = nullptr);
	void add_variables(const std::list<Variable_Stub>& _raw_values, Array_Variable* _parent = nullptr, const std::string& _section = "");

	void add_variables(const OMFL_Reader& _omfl_reader);

	void add_variable(Variable_Base* _variable);
	void exclude_variable(Variable_Base* _variable);

	Variable_Base* get_variable(const std::string& _variable_name);
	Variable_Manager* get_section(const std::string& _name);

};


#endif // VARIABLE_MANAGER_H
