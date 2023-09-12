#pragma once
#include <string>
#include <vector>
using namespace std;

string CreateRoom();
void DeleteCodeOffServer(string deleteCode);
vector<string> RefreshMembers(string mainRoomCode);
bool DoesURLExist();
void SetRound(string roomCode, string roundSet);
string GetRound(string roomCode);
string RequestRoomCode();
void RefreshLines();