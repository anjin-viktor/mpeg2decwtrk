#ifndef BOOL_EXPR_PARSER_H_
#define BOOL_EXPR_PARSER_H_


#include <boost/spirit/include/qi.hpp>

template <typename Iterator>
class BoolExprParser: public boost::spirit::qi::grammar<Iterator>
{
	public:
		BoolExprParser():
		BoolExprParser::base_type(expression)
		{
			namespace qi = boost::spirit::qi;

			expression = *qi::space >> term >> *( *qi::space >> ( '|' >> *qi::space >> term   | '+'  >> *qi::space >> term ));
			term       = *qi::space >> factor >> *( *qi::space >> '&' >> *qi::space >> factor);
			factor     = 'x' >> qi::uint_  | '(' >> *qi::space >> expression >> *qi::space >> ')';
		}

	private:
		boost::spirit::qi::rule<Iterator>     expression;
		boost::spirit::qi::rule<Iterator>     term;
		boost::spirit::qi::rule<Iterator>     factor;
};


#endif
