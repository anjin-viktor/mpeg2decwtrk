#include <iostream>
#include <iterator>

#include <boost/program_options.hpp>

#include "WatermarkInfo.h"
#include "UsersInfo.h"
#include "Key.h"

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
		Key privateKey = parseKeyFile(vm["key"].as<std::string>());
	}
	catch(std::exception& e) {
		std::cerr << "error: " << e.what() << "\n";
		return 1;
	}
	catch(...) {
		std::cerr << "Exception of unknown type!\n";
	}

	return 0;
}
