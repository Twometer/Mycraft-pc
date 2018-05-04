#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;
struct JSONDATA {
	int datatype;
	map<string, JSONDATA*>* properties;
	vector<JSONDATA*>* arrayData;
	string *stringValue;
};
enum JSONDATATYPE {
	Object,
	Array,
	String
};
class ChatParser
{

private:
	static string color2Tag(string colorname);
	static string* jsonData2String(JSONDATA* data, string colorcode);
	static bool isHex(char c);
	static JSONDATA* string2Data(string toparse, int *cursorpos);
public:
	ChatParser();
	~ChatParser();
	static string* parseJson(string* json);
};

