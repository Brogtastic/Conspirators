#pragma once
#include <string>
using namespace std;

string MainToServer(string function, string code, int num);
string CheckCode(string generatedCode);
void DeleteCodeOffServer(string deleteCode);
string MyServer(string mainRoomCode);
void FrankServer(int num);