#ifndef KEY_CREATOR_H_
#define KEY_CREATOR_H_

#include "UserInfo.h"
#include "Key.h"
#include "WatermarkInfo.h"


void createKeys(std::vector<UserInfo> &users, const std::vector<WatermarkEntry> &wtrkInfo, const Key &key);


#endif
