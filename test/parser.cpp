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

	expr = "x1 & x0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
//	std::cerr << " info: " << (int) (begin == expr.end()) << "  |"  << std::string(begin, expr.end()) << std::endl;
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());

	expr = "x1 & x0 + x2 | x3";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());

	expr = "x1 & (x0 + x2) | x3";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());

	expr = "x1 & ((x0 + x2)|x3)";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());

	expr = "x1 & x0 & x0 & x0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());

	expr = "x1";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);	
	BOOST_CHECK(begin == expr.end());


	expr = "x";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());

	expr = "()";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());

	expr = "x1 &";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());

	expr = "x1 & ()";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());

	expr = "x1 | x";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
}


BOOST_AUTO_TEST_SUITE_END()