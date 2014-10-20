/* 
 * File:   Client.cpp
 * Author: Dreamszhu
 * 
 * Created on October 17, 2014, 9:33 AM
 */

#include "Client.h"

Client::Client() {
}

Client::Client(const Client& orig) {
}

Client::~Client() {
}

void Client::run(const char* ip) {
	unsigned short serverPort = 6666;

	RakNet::RakPeerInterface *client = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(0, 0);

	client->Startup(1, &socketDescriptor, 1);

	bool ret = (client->Connect(ip, serverPort, NULL, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);

	RakAssert(ret);

	RakNet::Packet *packet;
	bool connected = false;

	unsigned char typeId;

	while (1) {
		packet = client->Receive();
		if (packet) {
			std::cout << "Receive from server" << std::endl;
			RakNet::BitStream stream(packet->data, packet->length, false);

			stream.Read(typeId);

			switch (typeId) {
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					connected = true;
					std::cout << "connect success" << std::endl;

					RakNet::RakString msg("hello world!");

					RakNet::BitStream sendStream;
					sendStream.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
					sendStream.Write(msg);

					uint32_t sends = client->Send(&sendStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
					std::cout << "send server msg:" << sends << std::endl;
					break;
				}
				case ID_USER_PACKET_ENUM:
				{
					RakNet::RakString msg;
					stream.Read(msg);
					std::cout << "Receive msg from server:" << msg << std::endl;
					break;
				}
				default:
					break;
			}
			client->DeallocatePacket(packet);
			if (!connected) {
				std::cout << "connect failed" << std::endl;
				break;
			}
		}
	}
	
	RakNet::RakPeerInterface::DestroyInstance(client);
}

