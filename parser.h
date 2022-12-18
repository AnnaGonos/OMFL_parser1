#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "Type_Manager.h"
#include "OMFL_Reader.h"
#include "Variable_Manager.h"

namespace omfl
{

	//	proxy
	class Parser final
	{
	private:
		friend Parser parse(const std::string& _raw_data);

	private:
		Variable_Manager* m_var_mgr = nullptr;
		Variable_Base* m_requested_variable = nullptr;

	private:
		bool m_is_root = false;
		bool m_valid = false;

	public:
		Parser();
		Parser(Parser&& _other);
		~Parser();

		Parser(const Parser& _other) = delete;

	private:
		Parser(Variable_Manager* _subsection);
		Parser(Variable_Base* _requested_variable);

	public:
		bool valid() const;

	public:
		Parser Get(const std::string& _name) const;

	public:
		bool IsString() const;
		bool IsInt() const;
		bool IsFloat() const;
		bool IsBool() const;
		bool IsArray() const;

	public:
		std::string AsString() const;
		int AsInt() const;
		float AsFloat() const;
		bool AsBool() const;
		Parser AsArray() const;
		Parser operator[](unsigned int _index) const;

		std::string AsStringOrDefault(const std::string& _default = "") const;
		int AsIntOrDefault(int _default = 0) const;
		float AsFloatOrDefault(float _default = 0.0f) const;
		bool AsBoolOrDefault(bool _default = false) const;

	};

	Parser parse(const std::string& _raw_data);

}

#endif // PARSER_H
