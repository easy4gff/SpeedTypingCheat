#ifndef SPEED_TYPING_CHEAT_H
#define SPEED_TYPING_CHEAT_H

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <ctime>
#include <conio.h>
#include <WinUser.h>


// Send text to clipboar
void setClipBoardString(std::string source){
	const size_t len = source.length() + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), source.c_str(), len);
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}

// Get text from clipboard
std::string getClipBoardString() {
	OpenClipboard(NULL);
	std::string result_string;
	HANDLE hData = GetClipboardData(CF_TEXT);
	char* chBuffer = (char*)GlobalLock(hData);
	result_string = chBuffer;
	GlobalUnlock(hData);
	CloseClipboard();
	return result_string;
}

// Get unicode text from clipboard
wchar_t * getClipBoardStringWcharT() {
	OpenClipboard(NULL);
	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	wchar_t * chBuffer = static_cast<wchar_t*>(GlobalLock(hData));
	GlobalUnlock(hData);
	CloseClipboard();
	return chBuffer;
}

double randomDouble(double lowerBound, double upperBound)
{
	return lowerBound + static_cast<double>(rand()) / static_cast<double>(RAND_MAX / upperBound);
}
// Sleepeng for random period of time
double RandomSleeping(double lowerBound, double upperBound)
{
	double randNum{randomDouble(lowerBound, upperBound)}; //{ lowerBound + static_cast<double>(rand()) / static_cast<double>(RAND_MAX / upperBound) };
	Sleep(randNum);
	return randNum;
}

void pushKey(KEYBDINPUT & kb, INPUT & ip, wchar_t character)
{
	kb.wScan = character;
	kb.dwFlags = KEYEVENTF_UNICODE;
	ip.type = INPUT_KEYBOARD;
	ip.ki = kb;
	::SendInput(1, &ip, sizeof(ip));
}

void releaseKey(KEYBDINPUT & kb, INPUT & ip, wchar_t character)
{
	kb.wScan = character;
	kb.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
	ip.type = INPUT_KEYBOARD;
	ip.ki = kb;
	::SendInput(1, &ip, sizeof(ip));
}

void makeMistake(KEYBDINPUT & kb, INPUT & ip)
{
	wchar_t character = rand() % (31) + 1072;
	kb.wScan = character;
	kb.dwFlags = KEYEVENTF_UNICODE;
	ip.type = INPUT_KEYBOARD;
	ip.ki = kb;
	::SendInput(1, &ip, sizeof(ip));

	kb.wScan = character;
	kb.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
	ip.type = INPUT_KEYBOARD;
	ip.ki = kb;
	::SendInput(1, &ip, sizeof(ip));

	Sleep(2000);

	kb.wScan = VK_BACK;
	kb.dwFlags = KEYEVENTF_UNICODE;
	ip.type = INPUT_KEYBOARD;
	ip.ki = kb;
	::SendInput(1, &ip, sizeof(ip));

	kb.wScan = VK_BACK;
	kb.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
	ip.type = INPUT_KEYBOARD;
	ip.ki = kb;
	::SendInput(1, &ip, sizeof(ip));
}

// Old implementation
/*void oldTyping(std::string const & tokenString)
{
	for (int i{ 0 }; i < tokenString.length(); ++i)
	{

		char character{ tokenString[i] };
		int isUpper = isupper(character);
		character = toupper(character);

		if (isUpper)
		{
			keybd_event(VK_SHIFT, 0, 0, 0);
			keybd_event(VkKeyScan(character), 0, 0, 0);
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VkKeyScan(character), 0, KEYEVENTF_KEYUP, 0);
		}
		else
		{
			keybd_event(VkKeyScan(character), 0, 0, 0);
			keybd_event(VkKeyScan(character), 0, KEYEVENTF_KEYUP, 0);
		}
	}
}*/

void startCheating(int secondsWait, int minSleepDelay, int maxSleepDelay, int cntMistakes)
{
	wchar_t * tokenString{ getClipBoardStringWcharT() };

	for (int i{ secondsWait }; i > 0; --i)
	{
		Sleep(1000);
	}

	for (int i{ 0 }; i < wcslen(tokenString); ++i)
	{
		double rnd_wait{ RandomSleeping(minSleepDelay, maxSleepDelay) }; // 55, 100

		wchar_t character = tokenString[i];

		// Skipping "double-enter"
		if (character == '\r')
			continue;
		if (character == '\n')
			character = VK_SPACE;

		KEYBDINPUT kb = { 0 };
		INPUT ip = { 0 };

		double rnd_mistake{ randomDouble(0, 200) };
		if (rnd_mistake < cntMistakes) //4
			makeMistake(kb, ip);

		pushKey(kb, ip, character);
		releaseKey(kb, ip, character);
	}

	// Push ENTER
	KEYBDINPUT kb = { 0 };
	INPUT ip = { 0 };
	pushKey(kb, ip, VK_RETURN);
	releaseKey(kb, ip, VK_RETURN);
}


#endif
