#include "Key.h"

#include <cassert>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <libxml++/libxml++.h>


class KeyParser: public xmlpp::SaxParser
{
	public:
		KeyParser(Key *pkey): 
			m_pkey(pkey)
		{
			*m_pkey = Key();
		}

		virtual ~KeyParser()
		{
		}

	protected:
		virtual void on_characters(const Glib::ustring& characters)
		{
			m_nodeContent += characters;
		}

		virtual void on_end_element(const Glib::ustring& name)
		{
			std::string nodeName(name);
			std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::toupper);

			boost::algorithm::trim(m_nodeContent);

			if(nodeName == "F")
				m_pkey -> m_function = m_nodeContent;
			else if(nodeName == "V")
				m_pkey -> m_lfsrInitValue = boost::dynamic_bitset<>(m_nodeContent);
			else if(nodeName == "P")
				m_pkey -> m_lfsrPolinom = m_nodeContent;

			m_nodeContent.clear();
		}

		Key               *m_pkey;
		std::string        m_nodeContent;
};



Key parseKeyXML(const std::string &fileName)
{
	Key result;
	KeyParser parser(&result);
	parser.set_substitute_entities(true);
	parser.parse_file(fileName);

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

