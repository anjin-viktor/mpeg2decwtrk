#include "WatermarkInfo.h"

#include <cassert>
#include <algorithm>
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <libxml++/libxml++.h>
#include <glibmm/convert.h> 


class WtrkInfoParser: public xmlpp::SaxParser
{
	public:
		virtual ~WtrkInfoParser()
		{
		}

		std::vector<WatermarkEntry>  m_info;

	protected:
		virtual void on_start_element(const Glib::ustring& name,
                                   const AttributeList& attributes)
		{
			std::string nodeName(name);
			boost::algorithm::trim(nodeName);
			std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);


			if(nodeName == "ENTRY")
				for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
				{
					try
					{
						std::string arrtName(iter -> name);
						boost::algorithm::trim(arrtName);
						std::transform(arrtName.begin(), arrtName.end(), arrtName.begin(), ::toupper);

						if(arrtName == "POSITION")
						{
							m_currentEntry.m_position = 
								boost::lexical_cast<std::size_t>(iter -> value);
							break;
						}	
					}
					catch(const Glib::ConvertError& ex)
					{
						throw std::runtime_error("WtrkInfoParser: file processed failed (" + ex.what() +")");
					}
				}
		}

		virtual void on_characters(const Glib::ustring& characters)
		{
			m_nodeContent += characters;
		}

		virtual void on_end_element(const Glib::ustring& name)
		{
			std::string nodeName(name);

			boost::algorithm::trim(nodeName);
			std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);

			boost::algorithm::trim(m_nodeContent);

			if(nodeName == "V")
			{
				if(m_currentEntry.m_v1 == boost::dynamic_bitset<>())
					m_currentEntry.m_v1 = 
						boost::dynamic_bitset<>(m_nodeContent);
				else
					m_currentEntry.m_v2 = 
						boost::dynamic_bitset<>(m_nodeContent);
			}
			else if(nodeName == "DEFAULT_VALUE")
			{
				m_currentEntry.m_defaultValue = 
					boost::dynamic_bitset<>(m_nodeContent);
			}
			else if(nodeName == "ENTRY")
			{
				m_info.push_back(m_currentEntry);
				m_currentEntry = WatermarkEntry();
			}

			m_nodeContent.clear();
		}

		WatermarkEntry     m_currentEntry;
		std::string        m_nodeContent;
};



std::vector<WatermarkEntry> parseWatermarkInfoXML(const std::string &fileName)
{
	WtrkInfoParser parser;
	parser.set_substitute_entities(true);
	parser.parse_file(fileName);

	return parser.m_info;
}

