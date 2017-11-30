#pragma comment(lib,"user32.lib")

#define _CRT_SECURE_NO_WARNINGS

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <ctime>
#include <conio.h>
#include <WinUser.h>
#include <cassert>


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
	double randNum{ randomDouble(lowerBound, upperBound) }; //{ lowerBound + static_cast<double>(rand()) / static_cast<double>(RAND_MAX / upperBound) };
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

void makeDoubleMistake(KEYBDINPUT & kb, INPUT & ip)
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

	makeMistake(kb, ip);

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

void startCheating(int secondsWait, int minSleepDelay, int maxSleepDelay, int mistakeChance, int doubleMistakeChance)
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

		//double rnd_mistake{ randomDouble(0, 100) };
		int rnd_mistake = rand() % 200;
		if (rnd_mistake < doubleMistakeChance) //4
			makeDoubleMistake(kb, ip);
		else if (rnd_mistake > (200 - mistakeChance)) //4
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

#pragma once

namespace STC {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::CheckBox^  enableMistake;



	private: System::Windows::Forms::Label^  delayBeforeStart;
	private: System::Windows::Forms::Label^  minDelay;
	private: System::Windows::Forms::Label^  singleMistake;



	private: System::Windows::Forms::Label^  maxDelay;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  complexMistake;
	private: System::Windows::Forms::TextBox^  complexMistakeText;


	private: System::Windows::Forms::TextBox^  singleMistakeText;

	private: System::Windows::Forms::TextBox^  maxDelayText;

	private: System::Windows::Forms::TextBox^  minDelayText;

	private: System::Windows::Forms::TextBox^  delayBeforeStartText;


	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->enableMistake = (gcnew System::Windows::Forms::CheckBox());
			this->delayBeforeStart = (gcnew System::Windows::Forms::Label());
			this->minDelay = (gcnew System::Windows::Forms::Label());
			this->singleMistake = (gcnew System::Windows::Forms::Label());
			this->maxDelay = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->complexMistakeText = (gcnew System::Windows::Forms::TextBox());
			this->singleMistakeText = (gcnew System::Windows::Forms::TextBox());
			this->maxDelayText = (gcnew System::Windows::Forms::TextBox());
			this->minDelayText = (gcnew System::Windows::Forms::TextBox());
			this->delayBeforeStartText = (gcnew System::Windows::Forms::TextBox());
			this->complexMistake = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(71, 223);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(134, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Start cheating!";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// enableMistake
			// 
			this->enableMistake->AutoSize = true;
			this->enableMistake->Enabled = false;
			this->enableMistake->Location = System::Drawing::Point(71, 123);
			this->enableMistake->Name = L"enableMistake";
			this->enableMistake->Size = System::Drawing::Size(140, 17);
			this->enableMistake->TabIndex = 1;
			this->enableMistake->Text = L"Enable making mistakes";
			this->enableMistake->UseVisualStyleBackColor = true;
			this->enableMistake->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
			// 
			// delayBeforeStart
			// 
			this->delayBeforeStart->AutoSize = true;
			this->delayBeforeStart->Location = System::Drawing::Point(24, 33);
			this->delayBeforeStart->Name = L"delayBeforeStart";
			this->delayBeforeStart->Size = System::Drawing::Size(119, 13);
			this->delayBeforeStart->TabIndex = 4;
			this->delayBeforeStart->Text = L"Delay before start (sec):";
			// 
			// minDelay
			// 
			this->minDelay->AutoSize = true;
			this->minDelay->Location = System::Drawing::Point(24, 62);
			this->minDelay->Name = L"minDelay";
			this->minDelay->Size = System::Drawing::Size(77, 13);
			this->minDelay->TabIndex = 5;
			this->minDelay->Text = L"Min delay (ms):";
			// 
			// singleMistake
			// 
			this->singleMistake->AutoSize = true;
			this->singleMistake->Enabled = false;
			this->singleMistake->Location = System::Drawing::Point(24, 154);
			this->singleMistake->Name = L"singleMistake";
			this->singleMistake->Size = System::Drawing::Size(137, 13);
			this->singleMistake->TabIndex = 6;
			this->singleMistake->Text = L"Chance of a single mistake:";
			// 
			// maxDelay
			// 
			this->maxDelay->AutoSize = true;
			this->maxDelay->Location = System::Drawing::Point(24, 88);
			this->maxDelay->Name = L"maxDelay";
			this->maxDelay->Size = System::Drawing::Size(80, 13);
			this->maxDelay->TabIndex = 7;
			this->maxDelay->Text = L"Max delay (ms):";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->complexMistakeText);
			this->groupBox1->Controls->Add(this->singleMistakeText);
			this->groupBox1->Controls->Add(this->maxDelayText);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->minDelayText);
			this->groupBox1->Controls->Add(this->delayBeforeStartText);
			this->groupBox1->Controls->Add(this->complexMistake);
			this->groupBox1->Controls->Add(this->delayBeforeStart);
			this->groupBox1->Controls->Add(this->minDelay);
			this->groupBox1->Controls->Add(this->enableMistake);
			this->groupBox1->Controls->Add(this->singleMistake);
			this->groupBox1->Controls->Add(this->maxDelay);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(283, 266);
			this->groupBox1->TabIndex = 8;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Properties";
			// 
			// complexMistakeText
			// 
			this->complexMistakeText->Enabled = false;
			this->complexMistakeText->Location = System::Drawing::Point(196, 178);
			this->complexMistakeText->Name = L"complexMistakeText";
			this->complexMistakeText->Size = System::Drawing::Size(56, 20);
			this->complexMistakeText->TabIndex = 13;
			this->complexMistakeText->Text = L"1";
			// 
			// singleMistakeText
			// 
			this->singleMistakeText->Enabled = false;
			this->singleMistakeText->Location = System::Drawing::Point(196, 151);
			this->singleMistakeText->Name = L"singleMistakeText";
			this->singleMistakeText->Size = System::Drawing::Size(56, 20);
			this->singleMistakeText->TabIndex = 12;
			this->singleMistakeText->Text = L"2";
			// 
			// maxDelayText
			// 
			this->maxDelayText->Location = System::Drawing::Point(196, 85);
			this->maxDelayText->Name = L"maxDelayText";
			this->maxDelayText->Size = System::Drawing::Size(56, 20);
			this->maxDelayText->TabIndex = 11;
			this->maxDelayText->Text = L"90";
			// 
			// minDelayText
			// 
			this->minDelayText->Location = System::Drawing::Point(196, 59);
			this->minDelayText->Name = L"minDelayText";
			this->minDelayText->Size = System::Drawing::Size(56, 20);
			this->minDelayText->TabIndex = 10;
			this->minDelayText->Text = L"45";
			// 
			// delayBeforeStartText
			// 
			this->delayBeforeStartText->Location = System::Drawing::Point(196, 33);
			this->delayBeforeStartText->Name = L"delayBeforeStartText";
			this->delayBeforeStartText->Size = System::Drawing::Size(56, 20);
			this->delayBeforeStartText->TabIndex = 9;
			this->delayBeforeStartText->Text = L"3";
			// 
			// complexMistake
			// 
			this->complexMistake->AutoSize = true;
			this->complexMistake->Enabled = false;
			this->complexMistake->Location = System::Drawing::Point(24, 181);
			this->complexMistake->Name = L"complexMistake";
			this->complexMistake->Size = System::Drawing::Size(149, 13);
			this->complexMistake->TabIndex = 8;
			this->complexMistake->Text = L"Chance of a complex mistake:";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(283, 266);
			this->Controls->Add(this->groupBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		try {
			int waitSeconds{ System::Convert::ToInt32(delayBeforeStartText->Text) };
			int minDelay{ System::Convert::ToInt32(minDelayText->Text) };
			int maxDelay{ System::Convert::ToInt32(maxDelayText->Text) };
			int chanceSingleMistake = (enableMistake->Checked) ? System::Convert::ToInt32(singleMistakeText->Text) : 0;
			int chanceComplexMistake = (enableMistake->Checked) ? System::Convert::ToInt32(complexMistakeText->Text) : 0;

			/*assert(waitSeconds >= 0);
			assert(minDelay >= 0);
			assert(maxDelay >= minDelay);
			assert((chanceSingleMistake >= 0) && (chanceSingleMistake <= 100));
			assert((chanceComplexMistake >= 0) && (chanceComplexMistake <= 100));*/
			if (waitSeconds < 0 || minDelay < 0 || maxDelay < minDelay)
				throw std::exception();

			this->Hide();
			startCheating(waitSeconds, minDelay, maxDelay, chanceSingleMistake, chanceComplexMistake);
			this->Show();
		}
		catch (...) {
			MessageBox::Show("Incorrect input!");
		}
	}
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (enableMistake->Checked)
		{
			singleMistake->Enabled = true;
			complexMistake->Enabled = true;
			singleMistakeText->Enabled = true;
			complexMistakeText->Enabled = true;
		}
		else
		{
			singleMistake->Enabled = false;
			complexMistake->Enabled = false;
			singleMistakeText->Enabled = false;
			complexMistakeText->Enabled = false;
		}
	}
	};
}