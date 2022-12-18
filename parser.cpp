#include "parser.h"

using namespace omfl;


Parser::Parser()
{

}

Parser::Parser(Parser&& _other)
{
	m_var_mgr = _other.m_var_mgr;
	_other.m_var_mgr = nullptr;
	m_requested_variable = _other.m_requested_variable;
	_other.m_requested_variable = nullptr;
	m_is_root = _other.m_is_root;
	_other.m_is_root = false;
	m_valid = _other.m_valid;
	_other.m_valid = false;
}

Parser::~Parser()
{
	if(!m_is_root)
		return;
	delete m_var_mgr;
}



Parser::Parser(Variable_Manager* _subsection)
{
	m_valid = true;
	m_var_mgr = _subsection;
}

Parser::Parser(Variable_Base* _requested_variable)
{
	m_valid = true;
	m_requested_variable = _requested_variable;
}

#include <iostream>

Parser omfl::parse(const std::string& _raw_data)
{
	static bool type_mgr_initialized = false;
	if(!type_mgr_initialized)
	{
		Type_Manager::register_type("int",
			{
				[](void* _data_vptr){ delete (int*)_data_vptr; },
				[](void* _variable_vptr, const std::string& _value_as_string){
											*(int*)_variable_vptr = std::stoi(_value_as_string);
										},
				[](void* _variable_vptr, void* _data_ptr){ *(int*)_variable_vptr = *(int*)_data_ptr; }
			}
		);
		Type_Manager::register_type("float",
			{
				[](void* _data_vptr){ delete (float*)_data_vptr; },
				[](void* _variable_vptr, const std::string& _value_as_string)
										{
											*(float*)_variable_vptr = std::stof(_value_as_string);
										},
				[](void* _variable_vptr, void* _data_ptr){ *(float*)_variable_vptr = *(float*)_data_ptr; }
			}
		);
		Type_Manager::register_type("bool",
			{
				[](void* _data_vptr){ delete (bool*)_data_vptr; },
				[](void* _variable_vptr, const std::string& _value_as_string)
										{
											if(_value_as_string == "true")
												*(bool*)_variable_vptr = true;
											else if(_value_as_string == "false")
												*(bool*)_variable_vptr = false;
										},
				[](void* _variable_vptr, void* _data_ptr){ *(float*)_variable_vptr = *(float*)_data_ptr; }
			}
		);
		Type_Manager::register_type("std::string",
			{
				[](void* _data_vptr){ delete (std::string*)_data_vptr; },
				[](void* _variable_vptr, const std::string& _value_as_string){ *(std::string*)_variable_vptr = _value_as_string; },
				[](void* _variable_vptr, void* _data_ptr){ *(std::string*)_variable_vptr = *(std::string*)_data_ptr; }
			}
		);

		type_mgr_initialized = true;
	}

	Parser result;

	OMFL_Reader reader;

	reader.parse(_raw_data);

	if(reader.error_log().size() > 0)
	{
		std::cout << reader.error_log() << "\n";
		return result;
	}

	result.m_var_mgr = new Variable_Manager;
	result.m_var_mgr->add_variables(reader);
	result.m_valid = true;

	return result;
}



bool Parser::valid() const
{
	return m_valid;
}



Parser Parser::Get(const std::string &_name) const
{
	unsigned int first_dot_pos = 0;
	for(; first_dot_pos < _name.size() && _name[first_dot_pos] != '.'; ++first_dot_pos) { }

	if(first_dot_pos < _name.size())
	{
		Variable_Manager* subsection = m_var_mgr->get_section(_name.substr(0, first_dot_pos));
		if(subsection != nullptr)
			return Parser(subsection).Get(_name.substr(first_dot_pos + 1));
		return Parser();
	}

	Variable_Manager* subsection = m_var_mgr->get_section(_name);
	if(subsection != nullptr)
		return Parser(subsection);

	Variable_Base* variable = m_var_mgr->get_variable(_name);
	if(variable != nullptr)
		return Parser(variable);

	return Parser();
}



bool Parser::IsString() const
{
	String_Variable* test = cast_variable<String_Variable>(m_requested_variable);
	return test != nullptr;
}

bool Parser::IsInt() const
{
	Int_Variable* test = cast_variable<Int_Variable>(m_requested_variable);
	return test != nullptr;
}

bool Parser::IsFloat() const
{
	Float_Variable* test = cast_variable<Float_Variable>(m_requested_variable);
	return test != nullptr;
}

bool Parser::IsBool() const
{
	Bool_Variable* test = cast_variable<Bool_Variable>(m_requested_variable);
	return test != nullptr;
}

bool Parser::IsArray() const
{
	Array_Variable* test = cast_variable<Array_Variable>(m_requested_variable);
	return test != nullptr;
}



std::string Parser::AsString() const
{
	return *(cast_variable<String_Variable>(m_requested_variable));
}

int Parser::AsInt() const
{
	return *(cast_variable<Int_Variable>(m_requested_variable));
}

float Parser::AsFloat() const
{
	return *(cast_variable<Float_Variable>(m_requested_variable));
}

bool Parser::AsBool() const
{
	return *(cast_variable<Bool_Variable>(m_requested_variable));
}

Parser Parser::AsArray() const
{
	return Parser(m_requested_variable);
}

Parser Parser::operator[](unsigned int _index) const
{
	Array_Variable* arrvar = cast_variable<Array_Variable>(m_requested_variable);

	if(arrvar == nullptr)
		return Parser();
	if(arrvar->get_childs().size() <= _index)
		return Parser();

	std::list<Variable_Base*>::iterator it = arrvar->get_childs().begin();
	for(unsigned int i=0; i<_index; ++i)
		++it;
	return Parser(*it);
}



std::string Parser::AsStringOrDefault(const std::string& _default) const
{
	if(!IsString())
		return _default;
	return AsString();
}

int Parser::AsIntOrDefault(int _default) const
{
	if(!IsInt())
		return _default;
	return AsInt();
}

float Parser::AsFloatOrDefault(float _default) const
{
	if(!IsFloat())
		return _default;
	return AsFloat();
}

bool Parser::AsBoolOrDefault(bool _default) const
{
	if(!IsBool())
		return _default;
	return AsBool();
}


















































