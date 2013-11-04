#ifndef USER_INFO_H_
#define USER_INFO_H_

#include <vector>
#include <string>

#include "Key.h"

class UserInfo
{
	public:
		std::string      m_id;
		Key              m_key;
};


std::vector<UserInfo> parseUsersInfoXML(const std::string &fileName);


#endif
