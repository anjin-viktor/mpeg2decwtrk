#include <iostream>
#include <iterator>
#include <fstream>
#include <stdexcept>

#include <boost/program_options.hpp>

#include <Key.h>
#include <LFSR.h>
#include <BoolCalc.h>

void process(const std::string &in, const std::string &out, const Key &key)
{
	std::ifstream fileIn(in.c_str(), std::ios_base::in | std::ios_base::binary);
	if(!fileIn)
		throw std::runtime_error("problem with opening file `" + in + "` for reading");

	std::ofstream fileOut(out.c_str(), std::ios_base::out | std::ios_base::binary);
	if(!fileOut)
		throw std::runtime_error("problem with opening file `" + out + "` for writing");

	char ch;
	fileIn.get(ch);

	LFSR lfsr(key.m_lfsrPolinom, key.m_lfsrInitValue);	
	bcc::Function f(key.m_function, bcc::Function::BDD, 
		bcc::Function(key.m_lfsrPolinom).getNumberOfVars());

	for(;!fileIn.eof();)
	{
		char gamma = 0;
		unsigned char mask = 1 << (sizeof(char) * CHAR_BIT - 1);
		for(;mask; lfsr.nextState(), mask >>= 1)
			if(f.calculate(lfsr.getLFSRVector()))
				gamma |= mask;

		fileOut.put(ch ^ gamma);
		fileIn.get(ch);
	}
}


int main(int argc, char **argv)
{
	namespace po = boost::program_options;
	try {
		po::options_description desc("Options");
		desc.add_options()
			("help", "produce help message")
			("input,i", po::value<std::string>(), 
				"path to file with watermark information (prodused by mpeg2wtrkinfo utils)")
			("output,o", po::value<std::string>(), 
				"path to file with informations about users")
			("key,k", po::value<std::string>(), "path to key file")
		;

		po::variables_map vm;        
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) 
		{
			std::cout << desc << "\n";
			return 0;
		}

		if(!vm.count("input") || !vm.count("output") || !vm.count("key"))
		{
			std::cout << desc << "\n";
			return 0;
		}

		Key privateKey = parseKeyXML(vm["key"].as<std::string>());
		process(vm["input"].as<std::string>(), vm["output"].as<std::string>(), privateKey);
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
