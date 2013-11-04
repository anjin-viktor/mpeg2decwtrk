#ifndef KEY_H_
#define KEY_H_

#include <string>

#include <boost/dynamic_bitset.hpp>

class Key
{
	public:
		std::string                m_function;
		std::string                m_lfsrPolinom;
		boost::dynamic_bitset<>    m_lfsrInitValue;
};

Key parseKeyXML(const std::string &fileName);
void writeKeyXML(const std::string &path, const Key &key);

#endif
