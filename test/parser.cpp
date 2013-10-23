#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/spirit/include/qi.hpp>
#include "BoolExprParser.h"


BOOST_AUTO_TEST_SUITE(parser);

namespace qi = boost::spirit::qi;

BOOST_AUTO_TEST_CASE(parse)
{
	BoolExprParser<std::string::iterator> parser;


	std::string expr("x1&x0");
	std::string::iterator begin = expr.begin();

	bool res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & x0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & x0 + x2 | x3";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & (x0 + x2) | x3";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & ((x0 + x2)|x3)";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & x0 & x0 & x0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);	
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "!x1";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);	
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "!0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);	
	BOOST_CHECK(begin == expr.end());
	parser.clear();


	expr = "x";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "()";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "x1 &";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "x1 & ()";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "x1 | x";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();
}


BOOST_AUTO_TEST_CASE(parse_exec_elementary)
{
	std::shared_ptr<Node> and_, or_, xor_, not_;

	BoolExprParser<std::string::iterator> parser;

	std::string expr("x0 & x1");

	qi::parse(expr.begin(), expr.end(), parser);
	and_ = parser.m_root;
	parser.clear();

	expr = "x0 | x1";

	qi::parse(expr.begin(), expr.end(), parser);
	or_ = parser.m_root;
	parser.clear();

	expr ="x0 + x1";

	qi::parse(expr.begin(), expr.end(), parser);
	xor_ = parser.m_root;
	parser.clear();

	expr = "!x0";

	qi::parse(expr.begin(), expr.end(), parser);
	not_ = parser.m_root;
	parser.clear();

	std::vector<bool> v(2);
	v[0] = false;
	v[1] = false;

	BOOST_CHECK_EQUAL(and_ -> exec(v), false);
	BOOST_CHECK_EQUAL(or_ -> exec(v), false);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), false);
	BOOST_CHECK_EQUAL(not_ -> exec(v), true);

	v[0] = true;
	v[1] = false;

	BOOST_CHECK_EQUAL(and_ -> exec(v), false);
	BOOST_CHECK_EQUAL(or_ -> exec(v), true);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), true);
	BOOST_CHECK_EQUAL(not_ -> exec(v), false);

	v[0] = false;
	v[1] = true;

	BOOST_CHECK_EQUAL(and_ -> exec(v), false);
	BOOST_CHECK_EQUAL(or_ -> exec(v), true);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), true);
	BOOST_CHECK_EQUAL(not_ -> exec(v), true);

	v[0] = true;
	v[1] = true;

	BOOST_CHECK_EQUAL(and_ -> exec(v), true);
	BOOST_CHECK_EQUAL(or_ -> exec(v), true);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), false);
	BOOST_CHECK_EQUAL(not_ -> exec(v), false);


}


BOOST_AUTO_TEST_SUITE_END()