#ifndef WATERMARK_INFO_H_
#define WATERMARK_INFO_H_

#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>

class WatermarkEntry
{
	public:
		boost::dynamic_bitset<> m_v1;
		boost::dynamic_bitset<> m_v2;
		boost::dynamic_bitset<> m_defaultValue;
		std::size_t             m_position;

};


std::vector<WatermarkEntry> parseWatermarkInfoXML(const std::string &fileName);

#endif
