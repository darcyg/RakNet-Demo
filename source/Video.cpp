/* 
 * File:   Video.cpp
 * Author: Dreamszhu
 * 
 * Created on October 20, 2014, 09:28 AM
 */

#include "Video.h"

Video::Video() {
}

Video::~Video() {
}

void Video::run() {
	unsigned int maxConnectionsAllowed = 4;
	unsigned int maxPlayersPerServer = 4;
	unsigned short serverPort = 7000;

	RakNet::RakPeerInterface *rakPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(serverPort, 0);

	if (rakPeer->Startup(maxConnectionsAllowed, &socketDescriptor, 1) != RakNet::RAKNET_STARTED) {
		std::cerr << "Startup fail:" << std::endl;
		RakNet::RakPeerInterface::DestroyInstance(rakPeer);
		return;
	}

	rakPeer->SetMaximumIncomingConnections(maxPlayersPerServer);

	char ip[256];
	std::cout << "Enter IP of remote system: " << std::endl;
	std::cin.getline(ip, sizeof (ip));
	std::cout << "ip: " << ip << std::endl;
	if (ip[0] != 0) {
		std::cout << "Connect: " << ip << std::endl;
		rakPeer->Connect(ip, serverPort, 0, 0);
	}

	cvNamedWindow("RemoteVideo", 1);
	cvNamedWindow("MyVideo", 1);
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

	RakNet::Packet *packet;
	unsigned char typeId;
	bool connected = false;
	char key;
	while (1) {
		IplImage* frame = cvQueryFrame(capture);
		cvShowImage("MyVideo", frame);
		key = cvWaitKey(10);
		if (char(key) == 27) {
			break;
		}

		packet = rakPeer->Receive();
		if (packet) {
			std::cout << "Receive data from" << packet->systemAddress.ToString() << std::endl;
			RakNet::BitStream bitStream(packet->data, packet->length, false);

			bitStream.Read(typeId);
			switch (typeId) {
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					connected = true;
					break;
				}
				case ID_USER_PACKET_ENUM:
				{
					IplImage recvFrame;
					bitStream.Read(recvFrame);
					
					cvShowImage("RemoteVideo", &recvFrame);

					std::cout << "Receive data" << std::endl;
					break;
				}
				default:
					break;
			}
			rakPeer->DeallocatePacket(packet);
		}

		if (connected) {
			std::cout << "Send data" << std::endl;
			RakNet::BitStream sendStream;
			sendStream.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
			sendStream.Write(frame);

			rakPeer->Send(&sendStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(rakPeer);

	cvReleaseCapture(&capture);
	cvDestroyWindow("MyVideo");
	cvDestroyWindow("RemoteVideo");
}
