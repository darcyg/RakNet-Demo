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
			<< boost::format("\t%-20s%s\n") % "-voice -a [ip]" % "Run voice"
			<< boost::format("\t%-20s%s\n") % "-video -v" % "Run video"
			<< std::endl;
}

char *l_opt_arg;
const char* short_options = "sc:a:v:";
struct option long_options[] = {
	{ "server", 0, NULL, 's'},
	{ "client", 1, NULL, 'c'},
	{ "voice", 1, NULL, 'a'},
	{ "video", 1, NULL, 'v'},
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
				l_opt_arg = optarg;
				std::cout << "Connect server run" << l_opt_arg << std::endl;
				Client client;
				client.run();
				break;
			}
			case 'a':
			{
				l_opt_arg = optarg;
				std::cout << "Voice run" << l_opt_arg << std::endl;
				Voice voice;
				voice.run();
				break;
			}
			case 'v':
			{
				l_opt_arg = optarg;
				std::cout << "Video run" << l_opt_arg << std::endl;
				Video video;
				video.run();
				break;
			}
			default:
			{				
				cvNamedWindow("Camera_Output", 1);
				CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
				char key;
				while (1) {
					IplImage* frame = cvQueryFrame(capture);
					cvShowImage("Camera_Output", frame);
					key = cvWaitKey(10);
					if (char(key) == 27) {
						break;
					}
				}
				cvReleaseCapture(&capture);
				cvDestroyWindow("Camera_Output");
				break;
			}
		}
	}
	return 0;
}

