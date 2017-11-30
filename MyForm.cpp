#include "MyForm.h"
#include <ctime>
//#include "SpeedTypingCheat.h";

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String^>^ args) {
	srand((unsigned)time(NULL));
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	STC::MyForm form;
	Application::Run(%form);
}