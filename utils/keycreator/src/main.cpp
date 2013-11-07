#include <iostream>
#include <iterator>
#include <fstream>

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
	try 
	{
		Config config;

		po::options_description generic("Generic options");
		generic.add_options()
			("help", "produce help message")
			("config,c", po::value<std::string>(), "name of a file of a configuration.")
		;

		po::options_description allowed("Configuration");
		allowed.add_options()
			("wtrk_info", po::value<std::string>(), 
				"path to file with watermark information (prodused by mpeg2wtrkinfo utils)")
			("users_info", po::value<std::string>(), 
				"path to file with informations about users")
			("key", po::value<std::string>(), "path to key")
			("out_dir,o", po::value<std::string>(), "path to out_directory")
			("max_affected_bits", po::value<std::size_t>(&(config.max_affected_bits)) -> default_value(1024), 
					"maximum number of bits, affected from watermarking")
		;

		po::options_description cmdOptions;
		cmdOptions.add(generic)
		          .add(allowed);

		po::options_description configOptions;
		configOptions.add(allowed);

        po::options_description visible("");
        visible.add(generic)
               .add(allowed);

		po::variables_map vm;        
        po::positional_options_description p;

		store(po::command_line_parser(argc, argv).
				options(cmdOptions).positional(p).run(), vm);
		notify(vm);

		if(vm.count("config"))
		{
			std::ifstream ifs(vm["config"].as<std::string>().c_str());
			if (!ifs)
			{
				std::cout << "Can not open config file: " << vm["config"].as<std::string>() << "\n";
				return 0;
			}
			else
			{
				store(parse_config_file(ifs, configOptions), vm);
				notify(vm);
			}
		}

		if (vm.count("help")) 
		{
			std::cout << visible << "\n";
			return 0;
		}

		if(!vm.count("wtrk_info") || !vm.count("users_info") || !vm.count("key") || 
				!vm.count("out_dir") || !vm.count("max_affected_bits"))
		{
			std::cout << visible << "\n";
			return 0;
		}


		config.max_affected_bits = vm["max_affected_bits"].as<std::size_t>();

		std::vector<WatermarkEntry> entries = parseWatermarkInfoXML(vm["wtrk_info"].as<std::string>());
		std::vector<UserInfo> users_info = parseUsersInfoXML(vm["users_info"].as<std::string>());
		Key privateKey = parseKeyXML(vm["key"].as<std::string>());

		createKeys(users_info, entries, config, privateKey);
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
