#include "UserInfo.h"

#include <cassert>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <libxml++/libxml++.h>
#include <glibmm/convert.h> 


class UsersInfoParser: public xmlpp::SaxParser
{
	public:
		virtual ~UsersInfoParser()
		{
		}

		std::vector<UserInfo>  m_info;

	protected:
		virtual void on_start_element(const Glib::ustring& name,
                                   const AttributeList& attributes)
		{
			std::string nodeName(name);
			boost::algorithm::trim(nodeName);
			std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);


			if(nodeName == "USER")
			{
				UserInfo currUser;
				for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
				{
					try
					{
						std::string arrtName(iter -> name);
						boost::algorithm::trim(arrtName);
						std::transform(arrtName.begin(), arrtName.end(), arrtName.begin(), ::toupper);

						if(arrtName == "ID")
						{
							currUser.m_id = iter -> value;
							break;
						}	
					}
					catch(const Glib::ConvertError& ex)
					{
						throw std::runtime_error("WtrkInfoParser: file processed failed (" + ex.what() +")");
					}
				}
				m_info.push_back(currUser);
			}
		}
};


std::vector<UserInfo> parseUsersInfoXML(const std::string &fileName)
{
	UsersInfoParser parser;
	parser.set_substitute_entities(true);
	parser.parse_file(fileName);

	return parser.m_info;
}
