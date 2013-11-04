#include <iostream>
#include <iterator>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "WatermarkInfo.h"
#include "UserInfo.h"
#include "Key.h"
#include "KeyCreator.h"

void saveKeys(const std::vector<UserInfo> &users_info, const std::string &directory)
{
	boost::filesystem::create_directory(directory);

	for(std::size_t i=0; i<users_info.size(); i++)
		writeKeyXML(directory + "/" + users_info[i].m_id, users_info[i].m_key);
}

int main(int argc, char **argv)
{
	namespace po = boost::program_options;
	try {
		po::options_description desc("Options");
		desc.add_options()
			("help", "produce help message")
			("wtrk_info", po::value<std::string>(), 
				"path to file with watermark information (prodused by mpeg2wtrkinfo utils)")
			("users_info", po::value<std::string>(), 
				"path to file with informations about users")
			("key", po::value<std::string>(), "path to key")
			("out_dir", po::value<std::string>(), "path to out_directory")
		;

		po::variables_map vm;        
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) 
		{
			std::cout << desc << "\n";
			return 0;
		}

		if(!vm.count("wtrk_info") || !vm.count("users_info") || !vm.count("key") || !vm.count("out_dir"))
		{
			std::cout << desc << "\n";
			return 0;
		}

		std::vector<WatermarkEntry> entries = parseWatermarkInfoXML(vm["wtrk_info"].as<std::string>());
		std::vector<UserInfo> users_info = parseUsersInfoXML(vm["users_info"].as<std::string>());
		Key privateKey = parseKeyXML(vm["key"].as<std::string>());

		createKeys(users_info, entries, privateKey);
		saveKeys(users_info, vm["out_dir"].as<std::string>());
	}
	catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	catch(...) {
		std::cerr << "Exception of unknown type!\n";
	}

	return 0;
}
