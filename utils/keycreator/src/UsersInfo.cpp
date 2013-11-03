#include "UsersInfo.h"

#include <cassert>
#include <algorithm>

#include <libxml++/libxml++.h>

static void processNode(const xmlpp::Node* node, std::vector<UserInfo> &usersInfo)
{
	std::string nodeName = node -> get_name();
	std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);
	if(nodeName == "USER")
	{
		UserInfo entry;

		const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
		if(!nodeElement)
		{
			assert(false);
			return;
		}

		const xmlpp::Element::AttributeList& attributes = nodeElement -> get_attributes();
		if(attributes.empty())
		{
			assert(false);
			return;
		}

		for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			const xmlpp::Attribute* attribute = *iter;

			std::string attrName = attribute -> get_name();
			std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::toupper);

			if(attrName == "ID")
			{
				entry.m_id = attribute -> get_value();
				break;
			}
		}

		usersInfo.push_back(entry);
	}
}

std::vector<UserInfo> parseUsersInfoXML(const std::string &fileName)
{
	xmlpp::DomParser parser;
	parser.parse_file(fileName);

	std::vector<UserInfo> result;
	if(parser)
	{
		const xmlpp::Node* pNode = parser.get_document() -> get_root_node();
		xmlpp::Node::NodeList list = pNode -> get_children();
		
		for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
			processNode(*iter, result);
	}

	return result;
}

