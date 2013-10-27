#include <BoolCalc.h>

#include "Node.h"
#include "BoolExprParser.h"

#include <boost/spirit/include/qi.hpp>

bcc::Function::Function(const std::string &expression)
{
	std::string expr = expression;
	BoolExprParser<std::string::iterator> parser;
	std::string::iterator itr = expr.begin();
	bool res = boost::spirit::qi::parse(itr, expr.end(), parser);

	if(!res || itr != expr.end())
		throw std::runtime_error("expression `" + expression + "` is incorrect");

	m_root = parser.m_root;
}


bool bcc::Function::calculate(const std::vector<bool> &values) const throw(std::runtime_error)
{
	if(!m_root)
		throw std::runtime_error("expression is empty");

	return m_root -> exec(values);
}


bool bcc::Function::calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error)
{
	if(!m_root)
		throw std::runtime_error("expression is empty");

	return m_root -> exec(values);
}
