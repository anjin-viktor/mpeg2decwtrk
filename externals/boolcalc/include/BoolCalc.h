#ifndef BOOL_CALC_H_
#define BOOL_CALC_H_

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

#include <boost/dynamic_bitset.hpp>

namespace bcc
{
	class Node;

	class Function
	{
		public:
			Function(const std::string &expression);
			bool calculate(const std::vector<bool> &values) const throw(std::runtime_error);
			bool calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error);

		protected:
			std::shared_ptr<bcc::Node>     m_root;
	};
}

#endif
