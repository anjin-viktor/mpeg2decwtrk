#include "LFSR.h"

#include <BoolCalc.h>


LFSR::LFSR(const std::string &polinom, const boost::dynamic_bitset<> &initVector):
m_vector(initVector),
m_ppolinom(new bcc::Function(polinom))
{
}

boost::dynamic_bitset<> LFSR::getLFSRVector() const
{
	return m_vector;
}


void LFSR::nextState()
{
	bool newValue = m_ppolinom -> calculate(m_vector);
	m_vector <<= 1;
	m_vector.set(0, newValue);
}
