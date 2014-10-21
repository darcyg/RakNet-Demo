/* 
 * File:   Video.cpp
 * Author: Dreamszhu
 * 
 * Created on October 20, 2014, 09:28 AM
 */

#include "Video.h"
#include "CheckSum.h"
#include <time.h>

Video::Video() {
}

Video::~Video() {
}

void Video::run(const char* ip) {
	unsigned int maxConnectionsAllowed = 1;
	unsigned int maxPlayersPerServer = 1;
	unsigned short serverPort = 7000;

	RakNet::RakPeerInterface *rakPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(serverPort, 0);

	if (rakPeer->Startup(maxConnectionsAllowed, &socketDescriptor, 1) != RakNet::RAKNET_STARTED) {
		std::cerr << "Startup fail:" << std::endl;
		RakNet::RakPeerInterface::DestroyInstance(rakPeer);
		return;
	}

	rakPeer->SetMaximumIncomingConnections(maxPlayersPerServer);

	RakNet::Packet *packet;
	unsigned char typeId;
	bool connected = false;
	char key;

	RakNet::SystemAddress address;

	if (ip) {
		std::cout << "Connect: " << ip << std::endl;
		rakPeer->Connect(ip, serverPort, 0, 0);
	}

	cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
	cv::createTrackbar("AlphaTrackbar", "MyVideo", &alpha_slider, 100);

	capture.open(0);
	if (!capture.isOpened()) {
		std::cout << "Capture open fail" << std::endl;
		return;
	}
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	capture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('X', '2', '6', '4'));

	cv::namedWindow("RemoteVideo", 1);

	while (1) {
		const time_t t = time(NULL);
		struct tm* current_time = localtime(&t);
		std::cout << "current time is " << current_time->tm_sec << std::endl;

		cv::Mat frame;
		capture.read(frame);
		cv::imshow("MyVideo", frame);

		packet = rakPeer->Receive();
		if (packet) {
			RakNet::BitStream bitStream(packet->data, packet->length, false);

			bitStream.Read(typeId);
			switch (typeId) {
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					std::cout << "ID_CONNECTION_REQUEST_ACCEPTED" << ip << std::endl;
					address = packet->systemAddress;
					connected = true;
					break;
				}
				case ID_NEW_INCOMING_CONNECTION:
				{
					std::cout << "ID_NEW_INCOMING_CONNECTION" << ip << std::endl;
					address = packet->systemAddress;
					connected = true;
					break;
				}
				case ID_DISCONNECTION_NOTIFICATION:
				{
					connected = false;
					break;
				}
				case ID_CONNECTION_LOST:
				{
					connected = false;
					break;
				}
				case ID_USER_PACKET_ENUM:
				{					
					int cols, rows, type, channels, size;
					
					bitStream.Read(cols);
					bitStream.Read(rows);
					bitStream.Read(type);
					bitStream.Read(channels);
					bitStream.Read(size);
					
					char* data = new char[size];
					bitStream.Read(data, size);
					
					cv::Mat mat(cols, rows, type, (uchar*)data);
							
					cv::imshow("RemoteVideo", mat.reshape(channels, rows));
					
					delete data;
					break;
				}
				default:
					break;
			}
			rakPeer->DeallocatePacket(packet);
		}

		if (connected) {
			int size = frame.total()*frame.elemSize();

			RakNet::BitStream sendStream;
			sendStream.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
			sendStream.Write(frame.cols);
			sendStream.Write(frame.rows);
			sendStream.Write((int)frame.type());
			sendStream.Write(frame.channels());
			sendStream.Write(size);
			sendStream.Write((const char *)frame.data, size);

			rakPeer->Send(&sendStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, 0, address, false);
			//rakPeer->Send(&sendStream, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		key = cvWaitKey(10);
		if (char(key) == 27) {
			break;
		}
	}

	rakPeer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(rakPeer);

	cv::destroyAllWindows();
}
