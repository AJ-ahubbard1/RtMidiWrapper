#pragma once
#include "../RtMidi/RtMidi.h"
#include "../MidiMessage/MidiMessage.h"
#include "MidiError.h"
#include "MidiInCallback.h"

namespace MidiInterface
{
	struct Range
	{
		ubyte low, high;
	};
	class MidiIn
	{
	public:
		MidiIn(bool useCallback = true)
		{
			mInstrument = new RtMidiIn();
			mInstrument->setErrorCallback(*midiErrorCallback, nullptr);
			mNumPorts = mInstrument->getPortCount();
			if (mNumPorts == 0)
			{
				std::cout << "No midiin ports.\n";
			} 
			else
			{
				fillPortNames();
				mInstrument->openPort(mPortNum);
				if (useCallback)
				{
					mInstrument->setCallback(&midiInCallback);
				}
			}
		}
		~MidiIn()
		{
			mInstrument->closePort();
			delete mInstrument;
			std::cout << "MidiIn deleted\n";
		}

		void changePort(ubyte p)
		{
			mInstrument->closePort();
			mPortNum = p;
			mInstrument->openPort(mPortNum);
			mInstrument->setCallback(&midiInCallback);
		}

		unsigned int getNumPorts() const
		{
			return mNumPorts;
		}
		
		const std::vector<std::string>& getPortNames() const
		{
			return mPortNames;
		}
	
		bool checkForMessage()
		{
			static double timestamp;
			static std::vector<unsigned char> message;
			timestamp = mInstrument->getMessage(&message);
			if (message.size() > 0)
			{
				mMessage = MidiMessage(message[0], message[1], message[2]);
				mMessage.setTimestamp(timestamp);
				return true;
			}
			return false;
		}
		MidiMessage& getMessage()
		{
			return mMessage;
		}
	
	private:
		ubyte mPortNum{0};
		RtMidiIn* mInstrument;
		ubyte mNumPorts{0};
		Range mRange{36, 96};
		std::vector<std::string> mPortNames;
		MidiMessage mMessage;

		void fillPortNames()
		{
			for (int i = 0; i < mNumPorts; i++)
			{
				mPortNames.push_back(mInstrument->getPortName(i));
			}
		}

	};
}
