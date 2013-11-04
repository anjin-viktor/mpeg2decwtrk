#include "KeyCreator.h"

#include <vector>
#include <string>

#include <boost/dynamic_bitset.hpp>
#include <boost/lexical_cast.hpp>

#include "LFSR.h"


enum FeatureType
{
	Default,
	Zero,
	One
};


static std::vector<std::vector<FeatureType> > createFeatures(const std::vector<UserInfo> &users, std::size_t wtrkSize)
{
	std::vector<std::vector<FeatureType> > result(users.size());
	for(std::size_t i=0; i<result.size(); i++)
	{
		result[i].resize(wtrkSize);
		for(std::size_t k=0; k<wtrkSize; k++)
			result[i][k] = (k % users.size()) == i ? Zero : One;
	}

	return result;
}

static std::vector<boost::dynamic_bitset<> > createInternalKeys(const std::vector<WatermarkEntry> &wtrkInfo, 
	const std::vector<std::vector<FeatureType> > &features)
{
	std::vector<boost::dynamic_bitset<> > result(features.size());

	for(std::size_t i=0; i<result.size(); i++)
	{
		result[i] = boost::dynamic_bitset<> (wtrkInfo.rbegin() -> m_position + 
			wtrkInfo.rbegin() -> m_defaultValue.size());

		for(std::size_t k=0; k<features[i].size(); k++)
		{
			boost::dynamic_bitset<> delta;

			if(features[i][k] == Zero)
				if(wtrkInfo[k].m_v1 != wtrkInfo[k].m_defaultValue)
					delta = wtrkInfo[k].m_v1 ^ wtrkInfo[k].m_defaultValue;
			else if(features[i][k] == One)
				if(wtrkInfo[k].m_v2 != wtrkInfo[k].m_defaultValue)
					 delta = wtrkInfo[k].m_v2 ^ wtrkInfo[k].m_defaultValue;

			if(delta.size() && delta.any())
				for(std::size_t l=0; l<delta.size(); l++)
					if(delta.test(delta.size() - 1 - l))
						result[i].set(wtrkInfo[k].m_position + l, 1);

		}
	}

	return result;
}


static std::string monomToStr(const boost::dynamic_bitset<> &monom)
{
	std::string result;

	if(monom.test(0))
		result = "x0";
	else
		result = "!x0";

	for(std::size_t i=1; i<monom.size(); i++)
		if(monom.test(i))
			result += " & x" + boost::lexical_cast<std::string>(i); 
		else
			result += " & !x" + boost::lexical_cast<std::string>(i); 

	return result;
}


void createKeys(std::vector<UserInfo> &users, const std::vector<WatermarkEntry> &wtrkInfo, const Key &key)
{
	for(std::size_t i=0; i<users.size(); i++)
		users[i].m_key = key;
	std::vector<std::vector<FeatureType> > features = createFeatures(users, wtrkInfo.size());
	std::vector<boost::dynamic_bitset<> > internalKeys = createInternalKeys(wtrkInfo, 
		createFeatures(users, wtrkInfo.size()));
	LFSR lfsr(key.m_lfsrPolinom, key.m_lfsrInitValue);

	for(std::size_t i=0; i<internalKeys[0].size(); i++)
		for(std::size_t k=0; k<users.size(); k++)
			if(internalKeys[k].test(i))
				users[k].m_key.m_function += " + " + monomToStr(lfsr.getLFSRVector());
}
