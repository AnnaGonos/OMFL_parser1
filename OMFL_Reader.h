#ifndef OMFL_READER_H
#define OMFL_READER_H

#include <string>
#include <list>
#include <map>
#include <fstream>


struct Variable_Stub
{
	std::string name;
	std::string value;
	std::list<Variable_Stub> childs;
	std::string comment;

	bool ok() { return value.size() > 0; }
	bool is_section() { return !ok() && name.size() > 0; }
};


class OMFL_Reader
{
public:
	using Parsed_Data_t = std::map<std::string, std::list<Variable_Stub>>;

private:
	Parsed_Data_t m_parsed_data;

private:
	std::string m_current_section;

private:
	std::string m_error_log;

private:
	void M_append_error_message(const std::string& _msg);
	std::string M_construct_syntax_error_message(unsigned int _line_number, const std::string& _description, const std::string& _line/*, unsigned int _error_pos*/) const;

public:
	const std::string& error_log() const;

private:
	Variable_Stub M_parse_line(const std::string& _line) const;
	std::string M_parse_word(const std::string& _line, unsigned int _offset) const;
	void M_parse_section(const std::string& _line, Variable_Stub& _result) const;
	void M_parse_variable(const std::string& _line, Variable_Stub& _result) const;
	std::string M_parse_string(const std::string& _line) const;
	void M_parse_array(const std::string& _array, Variable_Stub& _result) const;
	std::string M_parse_array_variable(const std::string& _array, unsigned int& _offset) const;
	std::string M_parse_whole_array_string(const std::string _array) const;
	std::string M_parse_variable_name(const std::string& _line) const;
	bool M_variable_name_is_valid(const std::string& _name) const;
	bool M_section_name_is_valid(const std::string& _name) const;
	unsigned int M_find_next_sumbol_pos(const std::string& _str, unsigned int _offset, char _symbol) const;
	bool M_only_one_value_specified(const std::string& _line, unsigned int _equals_sign_pos) const;
	std::string M_get_last_subsection_name(const std::string& _section_path) const;
	bool M_is_numeric_value_correct(const std::string& _value) const;
	bool M_is_value_correct(const std::string& _value) const;
	bool M_is_variable_registred(const std::string& _name, const std::list<Variable_Stub>& _section) const;

public:
	void parse(const std::string& _raw_data);
	void load_file(const std::string& _path);

	const Parsed_Data_t& parsed_data() const;

};


#endif // OMFL_READER_H
