#pragma once
#include <string>
#include <vector>
#include <thread>
using namespace std;

void CreateRoom();
void DeleteCodeOffServer(string deleteCode);
bool DoesURLExist();
void SetRound(string roundSet);
void PerformSSE();
void UpdateMembers();
void UpdateGameStage();