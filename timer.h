#pragma once
#define delta deltaTime()
#define fixed_delta fixedDeltaTime()

void initDeltaTime();
void setDeltaTime();
float deltaTime();
void setFixedDeltaTime();
float fixedDeltaTime();
bool timer(int number, float interval);
void resetTimer(int number);