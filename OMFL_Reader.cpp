#include "OMFL_Reader.h"


void OMFL_Reader::M_append_error_message(const std::string &_msg)
{
	if(m_error_log.size() > 0)
		m_error_log += "\n\n";

	m_error_log += _msg;
}

std::string OMFL_Reader::M_construct_syntax_error_message(unsigned int _line_number, const std::string& _description, const std::string &_line/*, unsigned int _error_pos*/) const
{
	std::string result = "Syntax error in line " + std::to_string(_line_number) + ":\n";
	if(_description.size() > 0)
		result += _description + '\n';
	std::string underline;
	underline.resize(_line.size());
	for(unsigned int i=0; i<underline.size(); ++i)
		underline[i] = '-';
	result += "  " + underline + '\n';
	result += "  " + _line + '\n';
	result += "  " + underline;

	return result;
}



const std::string& OMFL_Reader::error_log() const
{
	return m_error_log;
}



Variable_Stub OMFL_Reader::M_parse_line(const std::string &_line) const
{
	Variable_Stub result;

	if(_line.size() == 0)
		return result;

	unsigned int first_equals_pos = 0;
	unsigned int end = first_equals_pos + 1;
	for(; end < _line.size() && _line[end] != '\n'; ++end)
	{
		if(_line[end] == '=' && first_equals_pos == 0)
			first_equals_pos = end;
	}

	std::string word = M_parse_word(_line, 0);

	if(word[0] == '#')	//	comment
		return result;

	if(word[0] == '[')	//	section
	{
		M_parse_section(_line, result);
		return result;
	}

	if(first_equals_pos == 0)	//	no equals sign found (invalid syntax)
	{
		result.comment += "No equals sign found";
		return result;
	}

	if(word[0] == '=')
	{
		result.comment += "Parentless variable cannot be unnamed";
		return result;
	}

	word = M_parse_variable_name(_line);

	if(!M_variable_name_is_valid(word))	// chess speaks for itself
	{
		result.comment += "Invalid variable name: \"" + word + "\"";
		word = M_parse_variable_name(_line);
		return result;
	}

	std::string value = M_parse_word(_line, first_equals_pos + 1);

	if(value.size() == 0)	//	variable is empty
	{
		result.comment += "Variable \"" + word + "\" has no value";
		return result;
	}

	if(!M_only_one_value_specified(_line, first_equals_pos))
	{
		result.comment += "Invalid value specification for variable \"" + word + '\"';
		return result;
	}

	result.name = word;

	if(value[0] == '[')	//	variable is array
	{
		std::string array_string = M_parse_whole_array_string(_line.substr(first_equals_pos + 1));
		if(array_string.size() > 0)
			M_parse_array(array_string, result);
		else
			result.comment += "Invalid declaration of array \"" + word + "\"";
		return result;
	}
	if(value[0] == '\"')	//	variable is string
	{
		result.value = M_parse_string(_line.substr(first_equals_pos + 1));
		if(result.value.size() == 0)
			result.name = "";
		return result;
	}

	if(!M_is_numeric_value_correct(value))
	{
		result.comment = "Invalid numeric value of variable \"" + word + "\"";
		return result;
	}

	result.value = value;
	return result;
}

std::string OMFL_Reader::M_parse_word(const std::string &_line, unsigned int _offset) const
{
	for(; _offset < _line.size() && (_line[_offset] == ' ' || _line[_offset] == '\t'); ++_offset) { }

	if(_offset == _line.size())
		return "";

	unsigned int end = _offset + 1;
	for(; end < _line.size() && (_line[end] != ' ' && _line[end] != '\t'); ++end) { }

	return _line.substr(_offset, end - _offset);
}

void OMFL_Reader::M_parse_section(const std::string &_line, Variable_Stub &_result) const
{
	unsigned int offset = 0;
	for(; offset < _line.size() && _line[offset] != '['; ++offset) { }
	++offset;

	if(offset >= _line.size())
		return;

	unsigned int end = offset + 1;
	for(; end < _line.size() && _line[end] != ']'; ++end) { }

	if(offset >= end || end >= _line.size())
	{
		_result.comment += "Invalid section declaration: " + _line + '\"';
		return;
	}

	_result.name = _line.substr(offset, end - offset);
	_result.value = "";

	if(!M_section_name_is_valid(_result.name))
		_result.comment += "Invalid section name: \"" + _result.name + '\"';
}

std::string OMFL_Reader::M_parse_string(const std::string &_line) const
{
	unsigned int offset = 0;
	for(; offset < _line.size() && _line[offset] != '\"'; ++offset) { }

	if(offset >= _line.size())
		return "";

	unsigned int end = offset + 1;
	for(; end < _line.size() && _line[end] != '\"'; ++end) { }

	if(end >= _line.size())
		return "";

	return _line.substr(offset, end + 1 - offset);
}

void OMFL_Reader::M_parse_array(const std::string &_array, Variable_Stub &_result) const
{
	if(_array == "[]")
		return;

	unsigned int offset = 1;
	while(offset < _array.size())
	{
		std::string variable = M_parse_array_variable(_array, offset);

		if(_array[offset - 1] != ',')
			offset = M_find_next_sumbol_pos(_array, offset, ',') + 1;

		if(variable.size() == 0)
		{
			_result.comment += "Unnamed array variable has no value";
			_result.childs.clear();
			return;
		}

		Variable_Stub stub;
		if(variable[0] == '[')
		{
			M_parse_array(variable, stub);
			if(stub.childs.size() == 0 && stub.comment.size() > 0)
			{
				_result.childs.clear();
				return;
			}
			if(stub.comment.size() > 0)
			{
				_result.comment += stub.comment;
				_result.childs.clear();
				return;
			}
		}
		else
		{
			if(!M_is_value_correct(variable))
			{
				_result.comment += "Invalid array variable value: \"" + variable + '\"';
				_result.childs.clear();
				return;
			}
			stub.value = variable;
		}

		_result.childs.push_back(stub);
	}

	_result.value = "[]";
}

std::string OMFL_Reader::M_parse_array_variable(const std::string &_array, unsigned int& _offset) const
{
	unsigned int first_symbol_pos = _offset;
	for(; first_symbol_pos < _array.size() && (_array[first_symbol_pos] == ' ' || _array[first_symbol_pos] == '\t'); ++first_symbol_pos) { }

	std::string value;

	if(_array[first_symbol_pos] == '\"')
		value = M_parse_string(_array.substr(first_symbol_pos, _array.size()));
	else if(_array[first_symbol_pos] == '[')
		value = M_parse_whole_array_string(_array.substr(first_symbol_pos, _array.size()));

	if(value.size() > 0)
	{
		_offset = first_symbol_pos + value.size() + 1;
		return value;
	}

	unsigned int comma_pos = first_symbol_pos + 1;
	for(; comma_pos < _array.size() && _array[comma_pos] != ',' && _array[comma_pos] != ']'; ++comma_pos) { }
	_offset = comma_pos + 1;

	if(comma_pos == _array.size())
		return "";

	value = _array.substr(first_symbol_pos, comma_pos - first_symbol_pos);
	return value;
}

std::string OMFL_Reader::M_parse_whole_array_string(const std::string _array) const
{
	unsigned int skip_close_brackets = 0;

	unsigned int offset = 0;
	for(; offset < _array.size() && _array[offset] != '['; ++offset) { }

	if(offset == _array.size())
		return "";

	unsigned int end = offset + 1;
	for(; end < _array.size(); ++end)
	{
		if(_array[end] == '[')
			++skip_close_brackets;

		if(_array[end] == ']')
		{
			if(skip_close_brackets == 0)
				break;
			else
				--skip_close_brackets;
		}
	}

	if(end == _array.size())
		return "";

	return _array.substr(offset, end + 1);
}

std::string OMFL_Reader::M_parse_variable_name(const std::string &_line) const
{
	if(_line.size() == 0)
		return "";

	std::string result;

	unsigned int offset = 0;
	for(; offset < _line.size() && (_line[offset] == ' ' || _line[offset] == '\t'); ++offset);

	if(offset == _line.size())
		return "";

	if(_line[offset] == '=')
		return "";

	unsigned int end = offset + 1;
	for(; end < _line.size() && _line[end] != '='; ++end);
	--end;

	while((_line[end] == ' ' || _line[end] == '\t') && end > offset)
		--end;

	unsigned int size = end - offset + 1;
	return _line.substr(offset, size);
}

bool OMFL_Reader::M_variable_name_is_valid(const std::string &_name) const
{
	if(_name.size() == 0)
		return false;

	for(unsigned int i=0; i<_name.size(); ++i)
	{
		if( ! ( (_name[i] >= 'A' && _name[i] <= 'Z') || (_name[i] >= 'a' && _name[i] <= 'z') || (_name[i] >= '0' && _name[i] <= '9') || _name[i] == '-' || _name[i] == '_' ) )
			return false;
	}

	return true;
}

bool OMFL_Reader::M_section_name_is_valid(const std::string &_name) const
{
	if(_name.size() == 0)
		return false;

	for(unsigned int i=0; i<_name.size(); ++i)
	{
		if( ! ( (_name[i] >= 'A' && _name[i] <= 'Z') || (_name[i] >= 'a' && _name[i] <= 'z') || (_name[i] >= '0' && _name[i] <= '9') || _name[i] == '-' || _name[i] == '_' || _name[i] == '.') )
			return false;
	}

	if(_name[0] == '.')
		return false;

	unsigned int prev_dot_pos = _name.size(), current_dot_pos = _name.size();
	for(unsigned int i=0; i<_name.size(); ++i)
	{
		if(_name[i] == '.')
		{
			prev_dot_pos = current_dot_pos;
			current_dot_pos = i;

			if(prev_dot_pos == current_dot_pos - 1)
				return false;
		}
	}
	if(current_dot_pos == _name.size() - 1)
		return false;

	return true;
}

unsigned int OMFL_Reader::M_find_next_sumbol_pos(const std::string &_str, unsigned int _offset, char _symbol) const
{
	for(; _offset < _str.size() && _str[_offset] != _symbol; ++_offset) { }
	return _offset;
}

bool OMFL_Reader::M_only_one_value_specified(const std::string &_line, unsigned int _equals_sign_pos) const
{
	unsigned int offset = _equals_sign_pos + 1;
	for(; offset < _line.size() && (_line[offset] == ' ' || _line[offset] == '\t'); ++offset) {}

	if(offset == _line.size())
		return false;

	if(_line[offset] == '\"')
	{
		++offset;
		for(; offset < _line.size() && _line[offset] != '\"'; ++offset) {}

		if(offset == _line.size())
			return false;

		++offset;
	}
	else if(_line[offset] == '[')
	{
		unsigned int skip_close_brackets = 0;

		++offset;
		for(; offset < _line.size(); ++offset)
		{
			if(_line[offset] == '[')
				++skip_close_brackets;
			if(_line[offset] == ']')
			{
				if(skip_close_brackets == 0)
					break;
				else
					--skip_close_brackets;
			}
		}
		if(offset == _line.size())
			return false;

		++offset;
	}
	else
	{
		for(; offset < _line.size() && _line[offset] != ' ' && _line[offset] != '\t'; ++offset) {}
	}

	for(; offset < _line.size(); ++offset)
	{
		if(_line[offset] == '#')
			return true;
		if(_line[offset] != ' ' && _line[offset] != '\t')
			return false;
	}

	return true;
}

std::string OMFL_Reader::M_get_last_subsection_name(const std::string &_section_path) const
{
	unsigned int last_dot_pos = 0;
	for(unsigned int i = 0; i < _section_path.size(); ++i)
		if(_section_path[i] == '.')
			last_dot_pos = i + 1;

	return _section_path.substr(last_dot_pos);
}

bool OMFL_Reader::M_is_numeric_value_correct(const std::string &_value) const
{
	if(_value.size() == 0)
		return false;

	if(_value == "true" || _value == "false")
		return true;

	bool skip_first = _value[0] == '-' || _value[0] == '+';
	if(_value.size() == 1 && skip_first)
		return false;

	unsigned int dots_amount = 0;
	unsigned int first_dot_pos = _value.size();

	bool only_digits = true;

	for(unsigned int i=skip_first; i < _value.size(); ++i)
	{
		if(first_dot_pos == _value.size() && _value[i] == '.')
		{
			first_dot_pos = i;
			++dots_amount;
			continue;
		}
		if(!(_value[i] >= '0' && _value[i] <= '9'))
			only_digits = false;
	}

	if(!only_digits)
		return false;

	if(dots_amount > 1)
		return false;

	if(first_dot_pos == 0 || (first_dot_pos == 1 && skip_first))
		return false;

	if(first_dot_pos == _value.size() - 1)
		return false;

	return true;
}

bool OMFL_Reader::M_is_value_correct(const std::string &_value) const
{
	if(_value.size() == 0)
		return false;

	if(_value[0] == '\"' && _value[_value.size() - 1] == '\"')
		return true;
	if(_value == "true" || _value == "false")
		return true;

	if(M_is_numeric_value_correct(_value))
		return true;

	return false;
}

bool OMFL_Reader::M_is_variable_registred(const std::string &_name, const std::list<Variable_Stub> &_section) const
{
	std::list<Variable_Stub>::const_iterator it = _section.begin();
	while(it != _section.end())
	{
		if(it->name == _name)
			return true;
		++it;
	}
	return false;
}



void OMFL_Reader::parse(const std::string& _raw_data)
{
	m_parsed_data.clear();
	m_error_log.clear();

	unsigned int offset = 0;

	unsigned int line_number = 0;

	while(offset < _raw_data.size())
	{
		unsigned int next_line_offset = M_find_next_sumbol_pos(_raw_data, offset, '\n') + 1;
		std::string current_line = _raw_data.substr(offset, next_line_offset - 1 - offset);
		offset = next_line_offset;

		++line_number;

		if(current_line.size() == 0)
			continue;

		Variable_Stub stub = M_parse_line(current_line);

		if(stub.comment.size() > 0)
		{
			M_append_error_message(M_construct_syntax_error_message(line_number, stub.comment, current_line));
			continue;
		}

		if(stub.is_section())
		{
			m_current_section = stub.name;
			continue;
		}

		if(!stub.ok())
			continue;

		if(stub.name == M_get_last_subsection_name(m_current_section))
		{
			M_append_error_message(M_construct_syntax_error_message(line_number, "Invalid variable name (matches subsection name): \"" + stub.name + '\"', current_line));
			continue;
		}

		Parsed_Data_t::iterator section_it = m_parsed_data.find(m_current_section);
		if(section_it == m_parsed_data.end())
			section_it = m_parsed_data.emplace(m_current_section, std::list<Variable_Stub>()).first;

		if(M_is_variable_registred(stub.name, section_it->second))
		{
			M_append_error_message(M_construct_syntax_error_message(line_number, "Variable \"" + stub.name + "\" already exists", current_line));
			continue;
		}

		section_it->second.push_back(stub);
	}

	if(m_error_log.size() > 0)
		m_parsed_data.clear();
}

void OMFL_Reader::load_file(const std::string &_path)
{
	std::ifstream file(_path + ".omfl", std::ifstream::in);
	if(!file.is_open())
	{
		M_append_error_message("Invalid file path: " + _path + ".omfl");
		return;
	}

	file.seekg(0, std::ifstream::end);
	unsigned int size = file.tellg();
	file.seekg(0, std::ifstream::beg);

	std::string raw_content;
	raw_content.resize(size);
	unsigned int i = 0;
	for(; i<size && !file.eof(); ++i)
	{
		raw_content[i] = file.get();
		if(file.eof())
		{
			raw_content[i] = 0;
			break;
		}
	}
	raw_content.resize(i);

	file.close();
	parse(raw_content);
}


const OMFL_Reader::Parsed_Data_t& OMFL_Reader::parsed_data() const
{
	return m_parsed_data;
}
