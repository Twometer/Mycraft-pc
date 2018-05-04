#include "ChatParser.h"
#include <iostream>


ChatParser::ChatParser()
{
}


ChatParser::~ChatParser()
{
}

string* ChatParser::parseJson(string* json) {
	int cursorpos = 0;
	JSONDATA* jsonData = string2Data(*json, &cursorpos);
	return jsonData2String(jsonData, "");
}

string ChatParser::color2Tag(string colorname) {
	if (colorname == "black") return "§0";
	if (colorname == "dark_blue") return "§1";
	if (colorname == "dark_green") return "§2";
	if (colorname == "dark_aqua") return "§3";
	if (colorname == "dark_cyan") return "§3";
	if (colorname == "dark_red") return "§4";
	if (colorname == "dark_purple") return "§5";
	if (colorname == "dark_magenta") return "§5";
	if (colorname == "gold") return "§6";
	if (colorname == "dark_yellow") return "§6";
	if (colorname == "gray") return "§7";
	if (colorname == "dark_gray") return "§8";
	if (colorname == "blue") return "§9";
	if (colorname == "green") return "§a";
	if (colorname == "aqua") return "§b";
	if (colorname == "cyan") return "§b";
	if (colorname == "red") return "§c";
	if (colorname == "light_purple") return "§d";
	if (colorname == "magenta") return "§d";
	if (colorname == "yellow") return "§e";
	if (colorname == "white") return "§f";
	return "";
}

string* ChatParser::jsonData2String(JSONDATA *data, string colorcode) {
	string extra_result = "";
	if (data->datatype == Object) {
		if (data->properties->count("color")) {
			colorcode = color2Tag(*jsonData2String((*data->properties)["color"], ""));
		}
		if (data->properties->count("extra")) {
			vector<JSONDATA*> extras = *(*data->properties)["extra"]->arrayData;
			for (unsigned int i = 0; i < extras.size(); i++) {
				extra_result.append(*jsonData2String(extras.at(i), colorcode)).append("§r");
			}
		}
		if (data->properties->count("text")) {
			return new string(colorcode.append(*jsonData2String((*data->properties)["text"], colorcode)).append(extra_result));
		}
	}
	else if (data->datatype == Array) {
		string result = "";
		for (unsigned int i = 0; i < data->arrayData->size(); i++) {
			result.append(*(data->arrayData->at(i)->stringValue));
		}
	}
	else if (data->datatype == String) {
		return new string(colorcode.append(*data->stringValue));
	}
	return new string("<err>");
}
bool ChatParser::isHex(char c) {
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}
JSONDATA* ChatParser::string2Data(string toparse, int * cursorpos) {
	JSONDATA* data;
	if (*cursorpos >= toparse.length()) {
		data = new JSONDATA();
		data->datatype = String;
		return data;
	}
	char c = toparse[*cursorpos];
	if (c == '{') {
		data = new JSONDATA();
		data->datatype = Object;
		data->properties = new map<string, JSONDATA*>;
		(*cursorpos)++;
		while (*cursorpos < toparse.length() && toparse[*cursorpos] != '}') {
			if (toparse[*cursorpos] == '"') {
				JSONDATA* propertyname = string2Data(toparse, cursorpos);
				if (toparse[*cursorpos] == ':') (*cursorpos)++;
				JSONDATA* propertydata = string2Data(toparse, cursorpos);
				(*data->properties)[*propertyname->stringValue] = propertydata;
			}
			else (*cursorpos)++;
		}
		(*cursorpos)++;
	}
	else if (c == '[') {
		data = new JSONDATA();
		data->datatype = Array;
		data->arrayData = new vector<JSONDATA*>;
		(*cursorpos)++;
		while (*cursorpos < toparse.length() && toparse[*cursorpos] != ']') {
			if (toparse[*cursorpos] == ',') (*cursorpos)++;
			JSONDATA* arrayItem = string2Data(toparse, cursorpos);
			data->arrayData->push_back(arrayItem);
		}
		(*cursorpos)++;
	}
	else if (c == '"') {
		data = new JSONDATA();
		data->datatype = String;
		data->stringValue = new string;
		(*cursorpos)++;
		while (toparse[*cursorpos] != '"') {
			/*if (toparse[*cursorpos] == '\\') {
				if (toparse[(*cursorpos) + 1] == 'u'
					&& isHex(toparse[(*cursorpos) + 2])
					&& isHex(toparse[(*cursorpos) + 3])
					&& isHex(toparse[(*cursorpos) + 4])
					&& isHex(toparse[(*cursorpos) + 5])) {
					(*cursorpos) += 6;
				}
				else (*cursorpos)++;
			}*/
			char chr = toparse[*cursorpos];
			*data->stringValue += chr;
			(*cursorpos)++;
		}
		(*cursorpos)++;
	}
	else if (c == 't') {
		data = new JSONDATA;
		data->datatype = String;
		(*cursorpos)++;
		if (toparse[*cursorpos] == 'r') { (*cursorpos)++; }
		if (toparse[*cursorpos] == 'u') { (*cursorpos)++; }
		if (toparse[*cursorpos] == 'e') { (*cursorpos)++; data->stringValue = new string("true"); }
	}
	else if (c == 'f') {
		data = new JSONDATA;
		data->datatype = String;
		*cursorpos++;
		if (toparse[*cursorpos] == 'a') { *cursorpos++; }
		if (toparse[*cursorpos] == 'l') { *cursorpos++; }
		if (toparse[*cursorpos] == 's') { *cursorpos++; }
		if (toparse[*cursorpos] == 'e') {
			*cursorpos++; data->stringValue = new string("false");
		}
	}
	else {
		(*cursorpos)++;
		return string2Data(toparse, cursorpos);
	}
	return data;
}