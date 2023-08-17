#pragma once
#include <string>
using namespace std;

string CheckCode(string generatedCode);
void DeleteCodeOffServer(string deleteCode);
string GetMembers(string mainRoomCode, string order);
void TransferClicks(int num);
bool DoesURLExist(string roomCode);