#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "../RtMidiWrapper.h"

using namespace MidiInterface;

// Function Prototypes
static void case1();
static void case2();
static void case3();
struct Note
{
	ubyte pitch;
	double rhythm;
};

static void TestRtMidiWrapper(int option)
{
	switch (option)
	{
	case 1:
		case1();
		break;
	case 2:
		case2();
		break;
	case 3:
		case3();
		break;
	}

}

static void printPorts(const std::vector<std::string>& ports)
{
	unsigned int count = ports.size();
	for (unsigned int i = 0; i < count; i++)
	{
		std::cout << i << ": " << ports[i] << std::endl;
	}
}

static void case1()
{
	MidiOut player;
	auto& portNames = player.getPortNames();
	printPorts(portNames);

	// program change
	player.sendMessage(MidiMessage::ProgramChange(MidiInterface::SoundSet::DISTORTION_GUITAR));

	// ctrl change
	player.sendMessage(MidiMessage::ControlChange(7, 100));


	Note notes[]{
		{40,1},{42,1},{44,1},{45,2},
		{47,1},{49,1},{51,1},{52,3},
		{0, 1}, {40,4},
		{52,.667},{47,.667},{44,.667},{40,.667},
		{44,.667},{47,.667},{52,4}
	};

	int timePerBeat{150};

	for (auto& note : notes)
	{
		if (note.pitch)
		{
			player.sendMessage(MidiMessage::NoteOn(note.pitch, 90));
			player.sendMessage(MidiMessage::NoteOn(note.pitch + 12, 90));
			std::this_thread::sleep_for(std::chrono::milliseconds(int(timePerBeat * note.rhythm)));
			player.sendMessage(MidiMessage::NoteOff(note.pitch));
			player.sendMessage(MidiMessage::NoteOff(note.pitch + 12));
		} 
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(int(timePerBeat * note.rhythm)));
		}
	}
}
static void case2()
{
	MidiIn keyboard(true);
	auto& portNames = keyboard.getPortNames();
	printPorts(portNames);
	std::cout << "Reading Midi input ... press <enter> to quit.\n";
	char input;
	std::cin.get(input);

}

static void case3()
{
	MidiOut player;
	MidiIn keyboard(false);

	auto& portNames = keyboard.getPortNames();
	printPorts(portNames);
	std::cout << "Reading Midi input ...\n";
	while (true)
	{
		if (keyboard.checkForMessage())
		{
			MidiMessage mm = keyboard.getMessage();
			std::cout << "Message Received: " << mm.getString() << std::endl;
			player.sendMessage(keyboard.getMessage());
		}
	}
}