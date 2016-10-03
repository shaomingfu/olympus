#ifndef __GROUP_H__
#define __GROUP_H__

#include "item.h"
#include <vector>
#include <map>

using namespace std;

class group
{
public:
	vector<item> items;
	set<string> fs;
	MSS rs;

public:
	int load(const string &file);
	int forbid(const string &file);
	int replace(const string &file);
	int compare();
	int parse();
	int print();
};

#endif
