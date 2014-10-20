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

	cvNamedWindow("RemoteVideo", 1);
	cvNamedWindow("MyVideo", 1);
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, 2);

	RakNet::Packet *packet;
	unsigned char typeId;
	bool connected = false;
	char key;
	
	IplImage* frame;
	RakNet::SystemAddress address;

	int width;
	int height;
	int depth;
	int channels;
	int widthStep;
	int imageSize;
	char* imageData;

	if (ip) {
		std::cout << "Connect: " << ip << std::endl;
		rakPeer->Connect(ip, serverPort, 0, 0);
	}
	
	while (1) {
		frame = cvQueryFrame(capture);
		cvShowImage("MyVideo", frame);

		packet = rakPeer->Receive();
		while (packet) {
			std::cout << "Receive data from" << packet->systemAddress.ToString() << std::endl;
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
					bitStream.Read(width);
					bitStream.Read(height);
					bitStream.Read(depth);
					bitStream.Read(channels);
					bitStream.Read(widthStep);
					bitStream.Read(imageSize);

					imageData = (char*) malloc(imageSize);
					bitStream.Read(imageData, imageSize);

					IplImage* image = cvCreateImageHeader(cvSize(width, height), depth, channels);
					if (image) {
						cvSetData(image, imageData, widthStep);
						cvShowImage("RemoteVideo", image);
						cvReleaseImageHeader(&image);
					}

					free(imageData);
					
					std::cout << "Receive imageSize:" << imageSize << std::endl;
					break;
				}
				default:
					break;
			}
			rakPeer->DeallocatePacket(packet);
			packet = rakPeer->Receive();
		}

		if (connected && frame) {
			RakNet::BitStream sendStream;
			sendStream.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
			sendStream.Write(frame->width);
			sendStream.Write(frame->height);
			sendStream.Write(frame->depth);
			sendStream.Write(frame->nChannels);
			sendStream.Write(frame->widthStep);
			sendStream.Write(frame->imageSize);
			sendStream.Write(frame->imageData, frame->imageSize);
			
			std::cout << "Send imageSize:" << frame->imageSize << std::endl;

			rakPeer->Send(&sendStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);
			//rakPeer->Send(&sendStream, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}

		key = cvWaitKey(10);
		if (char(key) == 27) {
			break;
		}
	}
	rakPeer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(rakPeer);

	cvReleaseCapture(&capture);
	cvDestroyWindow("MyVideo");
	cvDestroyWindow("RemoteVideo");
}
