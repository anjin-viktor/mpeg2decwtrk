#ifndef KEY_CREATOR_H_
#define KEY_CREATOR_H_

#include "UserInfo.h"
#include "Key.h"
#include "WatermarkInfo.h"


struct Config
{
	std::size_t max_affected_bits;
};

void createKeys(std::vector<UserInfo> &users, const std::vector<WatermarkEntry> &wtrkInfo, 
	const Config &config, const Key &key);


#endif
