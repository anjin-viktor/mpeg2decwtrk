#ifndef KEY_H_
#define KEY_H_

#include <string>

#include <boost/dynamic_bitset.hpp>

class Key
{
	public:
		std::string                m_function;
		boost::dynamic_bitset<>    m_lfstInitValue;
};

Key parseKeyFile(const std::string &fileName);

#endif
