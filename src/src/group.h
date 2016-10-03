#ifndef __GROUP_H__
#define __GROUP_H__

#include "item.h"
#include "undirected_graph.h"

#include <vector>
#include <map>

using namespace std;

class group
{
public:
	vector<item> items;
	set<string> fs;
	MSS rs;
	undirected_graph gr;

public:
	int load(const string &file);
	int forbid(const string &file);
	int replace(const string &file);
	int build();
	int clust();
	int parse();
	int print();
};

bool set_compare(const set<int> &s1, const set<int> &s2);


#endif
