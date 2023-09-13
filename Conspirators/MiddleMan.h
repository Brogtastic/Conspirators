#pragma once
#include <string>
#include <vector>
#include <thread>
using namespace std;

string CreateRoom();
void DeleteCodeOffServer(string deleteCode);
vector<string> RefreshMembers(string mainRoomCode);
bool DoesURLExist();
void SetRound(string roomCode, string roundSet);
string GetRound(string roomCode);
void PerformSSE();
void CleanCurl();