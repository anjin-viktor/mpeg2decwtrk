#ifndef CONST_H_
#define CONST_H_


#include "Node.h"


class Const: public Node
{
	public:
		Const(bool value);
		bool exec(const std::vector<bool> &) const throw(std::runtime_error);

	private:
		bool     m_value;
};



#endif
