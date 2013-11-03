#include "WatermarkInfo.h"

#include <cassert>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include <libxml++/libxml++.h>


static void processNode(const xmlpp::Node* node, std::vector<WatermarkEntry> &wtrkInfo)
{
	std::string nodeName = node -> get_name();
	std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);
	if(nodeName == "ENTRY")
	{
		WatermarkEntry entry;

		const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
		if(!nodeElement)
		{
			assert(false);
			return;
		}

		const xmlpp::Element::AttributeList& attributes = nodeElement -> get_attributes();

		for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			std::string attrName = (*(attributes.begin())) -> get_name();
			std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::toupper);
			if(attrName == "POSITION")
			{
				entry.m_position = boost::lexical_cast<std::size_t>((*(attributes.begin())) -> get_value());
				break;
			}
		}

		xmlpp::Node::NodeList list = node->get_children();
		std::size_t numValues = 0;

		for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); iter++)
		{
			nodeName = (*iter) -> get_name();

			std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);

			if(nodeName == "V")
			{
				const xmlpp::ContentNode* nodeContent = 
					dynamic_cast<const xmlpp::ContentNode*>(*((*iter) -> get_children().begin()));

				if(numValues == 0)
					entry.m_v1 = boost::dynamic_bitset<>(std::string(nodeContent -> get_content()));
				else if(numValues == 1)
					entry.m_v2 = boost::dynamic_bitset<>(std::string(nodeContent -> get_content()));
				else
				{
					assert(false);
					return;
				}
				numValues++;
			}
		}

		wtrkInfo.push_back(entry);
	}
}

std::vector<WatermarkEntry> parseWatermarkInfoXML(const std::string &fileName)
{
	xmlpp::DomParser parser;
	parser.parse_file(fileName);

	std::vector<WatermarkEntry> result;
	if(parser)
	{
		const xmlpp::Node* pNode = parser.get_document() -> get_root_node();
		xmlpp::Node::NodeList list = pNode -> get_children();

		for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
			processNode(*iter, result);
	}

	return result;
}

