/* 
 * File:   Voice.h
 * Author: Dreamszhu
 *
 * Created on October 17, 2014, 10:52 AM
 */

#ifndef VOICE_H
#define	VOICE_H

#include <iostream> 
#include <portaudio.h>

#include "RakVoice.h"

#include "raknet/MessageIdentifiers.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"
#include "raknet/RakString.h"
#include "raknet/BitStream.h"

class Voice {
public:
	Voice();
	virtual ~Voice();
	
	void run(const char* ip);
	int portAudioCallback(void *inputBuffer, void *outputBuffer);
private:
	bool mute;
	RakNet::RakPeerInterface *rakPeer;
	RakNet::RakVoice rakVoice;
	PaError err;
	const int SAMPLE_RATE = 8000;
	const int FRAMES_PER_BUFFER = (2048 / (32000 / SAMPLE_RATE));
};

#endif	/* VOICE_H */

