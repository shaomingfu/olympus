#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

typedef pair<string, string> PSS;
typedef map<string, string> MSS;

class item
{
public:
	item();

public:
	string name;
	vector<string> words;
	int value;

public:
	bool build(const string &s);
	int parse(const set<string> &fs, MSS &rs);
	double similarity(const item &e);
	int print();

	static double similarity(const string &s1, const string &s2);
	static int distance(const string &s1, const string &s2);
	static int lcs(const string &s1, const string &s2);
};

#endif
