#include"framework.h"

void window(LPCSTR windowTitle, float baseWidth, float baseHeight, 
	bool windowed, int numDescriptors, bool centered)
{
	createWindow(windowTitle, baseWidth, baseHeight, windowed, centered);
	createGraphic(numDescriptors);
	createInput();
	showWindow();
}

void gmain();
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	timeBeginPeriod(1);

	gmain();

	waitGPU();
	closeEventHandle();
	destroyInput();
	timeEndPeriod(1);
	return msg_wparam();
}

void begin()
{
	setDeltaTime();
	getInputState();
	if (isTrigger(KEY_ESC))
	{
		closeWindow();
	}
	beginMsaaRender();
}

void end()
{
	endMsaaRender();
}

void winInfo()
{
	print("width:%.f height:%.f", width, height);
	print("mouseX:%.f mouseY:%.f", mouseX, mouseY);
	print("delta:%.3f", delta);
}

void descInfo()
{
	print("numDescriptors:%d", numConstants() + numLoadTextures() + numFontTextures());
	print("numConstants:%d", numConstants());
	print("numLoadTextures:%d", numLoadTextures());
	print("numFontTextures:%d", numFontTextures());
}

