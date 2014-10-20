/* 
 * File:   main.cpp
 * Author: Dreamszhu
 *
 * Created on October 17, 2014, 8:30 AM
 */

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "include/raknet/MessageIdentifiers.h"
#include "include/raknet/RakPeerInterface.h"
#include "include/raknet/RakNetTypes.h"
#include "Server.h"
#include "Client.h"
#include "Voice.h"
#include "Video.h"

static void show_usage(std::string name) {
	std::cerr << "RakNet Demo\n\n"
			<< "Usage: "
			<< "\tcommand [options]\n\n"
			<< boost::format("\t%-20s%s\n") % "-server -s" % "Run server"
			<< boost::format("\t%-20s%s\n") % "-client -c [ip]" % "Run client"
			<< boost::format("\t%-20s%s\n") % "-audio -a=[ip]" % "Run audio"
			<< boost::format("\t%-20s%s\n") % "-video -v=[ip]" % "Run video"
			<< std::endl;
}

char *ip = NULL;
const char* short_options = "sc:a::v::";
struct option long_options[] = {
	{ "server", no_argument, NULL, 's'},
	{ "client", required_argument, NULL, 'c'},	// 參數必選
	{ "audio", optional_argument, NULL, 'a'},	// 參數可選
	{ "video", optional_argument, NULL, 'v'},	// 參數可選
	{ 0, 0, 0, 0},
};

int main(int argc, char** argv) {
	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}

	int c;
	while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (c) {
			case 's':
			{
				std::cout << "Server run" << std::endl;
				Server server;
				server.run();
				break;
			}
			case 'c':
			{
				ip = optarg;
				std::cout << "client run:" << ip << std::endl;
				Client client;
				client.run(ip);
				break;
			}
			case 'a':
			{
				ip = optarg;
				std::cout << "audio run:" << ip << std::endl;
				Voice voice;
				voice.run(ip);
				break;
			}
			case 'v':
			{
				ip = optarg;
				std::cout << "Video run:" << ip << std::endl;
				Video video;
				video.run(ip);
				break;
			}
			default:
			{
				std::cerr << boost::format("%s: option `-%c` is invalid: ignored") % argv[0] % static_cast<char>(optopt) << std::endl;
				break;
			}
		}
	}
	return 0;
}

