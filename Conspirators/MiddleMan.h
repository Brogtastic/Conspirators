#pragma once
#include <string>
#include <vector>
using namespace std;

string CheckCode(string generatedCode);
void DeleteCodeOffServer(string deleteCode);
vector<string> RefreshMembers(string mainRoomCode);
void TransferClicks(int num);
bool DoesURLExist(string roomCode);
void SetRound(string roomCode, string roundSet);