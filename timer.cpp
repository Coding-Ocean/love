#include<windows.h>
#include"timer.h"

unsigned int PreTime = 0;
float DeltaTime = 0.0f;
float FixedDeltaTime = 0.0f;
float ElapsedTime[10] = {};

void initDeltaTime()
{
	PreTime = timeGetTime();
	DeltaTime = 0;
}
void setDeltaTime()
{
	unsigned int  curTime = timeGetTime();
	DeltaTime = (curTime - PreTime) / 1000.0f;
	PreTime = curTime;
}
void setFixedDeltaTime()
{
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	FixedDeltaTime = 1.0f / dm.dmDisplayFrequency;
}
bool timer(int number, float interval)
{
	ElapsedTime[number] += DeltaTime;
	if (ElapsedTime[number] >= interval) {
		ElapsedTime[number] -= interval;
		return true;
	}
	return false;
}
void resetTimer(int number)
{
	ElapsedTime[number] = 0;
}
float deltaTime()
{
	return DeltaTime;
}
float fixedDeltaTime()
{
	return FixedDeltaTime;
}


// 厳粛かつ正確かつ効率良く時間待ちをする
int iFPS = 60;
// dwFPSWaitは、待ち時間の小数以下を１６ビットの精度で持っていると考えよ
// これにより、double型を持ち出す必要がなくなる。
DWORD dwFPSWait = 1000 * 0x10000 / iFPS;
DWORD dwFPSWaitTT = 0;
void waitFPS(void)
{
	if (iFPS == 0) return; // Non-wait mode

	static DWORD lastdraw = 0; // 前回の描画時刻
	// （不運にも第一発目のtimeGetTime() == 0とかゆーこともあるが、それは構わない）

	DWORD t = timeGetTime(); // 現在時刻

	dwFPSWaitTT = (dwFPSWaitTT & 0xffff) + dwFPSWait; // 今回の待ち時間を計算


	DWORD dwWait = dwFPSWaitTT >> 16; // 結局のところ、今回は何ms待つねん？

	// １フレーム時間を経過しちょる。ただちに描画しなちゃい！
	DWORD dwElp = (DWORD)(t - lastdraw); // 前回描画からいくら経過しとんねん？
	if (dwElp >= dwWait) {
		lastdraw = t;
		return;
	}

	// ほな、時間を潰すとすっか！

	// まだ時間はたっぷりあるのか？
	// 4ms以上消費する必要があるのならば、Sleepする
	if (dwWait - dwElp >= 4) Sleep(dwWait - dwElp - 3);
	// いまdwWait>dwElpなのでdwWait-dwElp>=0と考えて良い

	// 95/98/NTで測定したところSleep(1);で1ms単位でスリープするのは可能
	// ただし、実装系依存の可能性もあるのでSleepの精度は3ms以内と仮定

	while ((timeGetTime() - lastdraw) < dwWait);
	// ループで時間を潰す（あまり好きじゃないけど）

	// これで、時間つぶし完了！

	lastdraw += dwWait; // ぴったりで描画が完了した仮定する。（端数を持ち込まないため）
}
