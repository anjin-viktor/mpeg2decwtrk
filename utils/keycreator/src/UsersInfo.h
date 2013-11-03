#ifndef USER_INFO_H_
#define USER_INFO_H_

#include <vector>
#include <string>

class UserInfo
{
	public:
		std::string      m_id;
};


std::vector<UserInfo> parseUsersInfoXML(const std::string &fileName);

#endif
