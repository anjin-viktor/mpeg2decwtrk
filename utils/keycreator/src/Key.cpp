#include "Key.h"

#include <cassert>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <libxml++/libxml++.h>

Key parseKeyFile(const std::string &fileName)
{
	xmlpp::DomParser parser;
	parser.parse_file(fileName);

	Key result;
	if(parser)
	{
		const xmlpp::Node* pNode = parser.get_document() -> get_root_node();
		std::string nodeName = pNode -> get_name();
		std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);

		if(nodeName == "KEY")
		{
			xmlpp::Node::NodeList list = pNode -> get_children();
			for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); iter++)
			{
				nodeName = (*iter) -> get_name();
				std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);

				if(nodeName == "F")
				{
					const xmlpp::ContentNode* nodeContent = 
						dynamic_cast<const xmlpp::ContentNode*>(*((*iter) -> get_children().begin()));
					result.m_function = nodeContent -> get_content();

					boost::algorithm::trim(result.m_function);
				}
				else if(nodeName == "K")
				{
					const xmlpp::ContentNode* nodeContent = 
						dynamic_cast<const xmlpp::ContentNode*>(*((*iter) -> get_children().begin()));

					std::string value = std::string(nodeContent -> get_content());
					boost::algorithm::trim(value);

					result.m_lfstInitValue = boost::dynamic_bitset<>(value);
				}

			}
		}
	}
	return result;
}


