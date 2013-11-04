#include "Key.h"

#include <cassert>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <libxml++/libxml++.h>

Key parseKeyXML(const std::string &fileName)
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

					result.m_lfsrInitValue = boost::dynamic_bitset<>(value);
				}
				else if(nodeName == "P")
				{
					const xmlpp::ContentNode* nodeContent = 
						dynamic_cast<const xmlpp::ContentNode*>(*((*iter) -> get_children().begin()));

					result.m_lfsrPolinom = std::string(nodeContent -> get_content());
					boost::algorithm::trim(result.m_lfsrPolinom);
				}
			}
		}
	}

	assert(result.m_lfsrInitValue.size() && result.m_lfsrPolinom.size() && result.m_function.size());
	return result;
}



void writeKeyXML(const std::string &path, const Key &key)
{
	xmlpp::Document document;
	xmlpp::Element* nodeRoot = document.create_root_node("key");

	xmlpp::Element *node = nodeRoot -> add_child("f");
	node -> add_child_text(key.m_function);
	nodeRoot -> add_child_text("\n");

	node = nodeRoot -> add_child("v");
	std::string str;
	to_string(key.m_lfsrInitValue, str);
	node -> add_child_text(str);
	nodeRoot -> add_child_text("\n");

	node = nodeRoot -> add_child("p");
	node -> add_child_text(key.m_lfsrPolinom);
	nodeRoot -> add_child_text("\n");

	document.write_to_file(path);
}
