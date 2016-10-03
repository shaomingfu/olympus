#include "group.h"
#include <fstream>
#include <iostream>
#include <sstream>

int group::load(const string &file)
{
	ifstream fin(file);
	if(fin.fail()) return 0;

	string s;
	while(getline(fin, s))
	{
		if(s.size() == 0) continue;
		item e;
		bool b = e.build(s);
		if(b == false) continue;
		items.push_back(e);
	}

	fin.close();
	return 0;
}

int group::forbid(const string &file)
{
	ifstream fin(file);
	if(fin.fail()) return 0;

	string s;
	while(getline(fin, s))
	{
		fs.insert(s);
	}
	fin.close();
	return 0;
}

int group::replace(const string &file)
{
	ifstream fin(file);
	if(fin.fail()) return 0;

	string s;
	while(getline(fin, s))
	{
		char buf1[10240];
		char buf2[10240];
		stringstream sstr(s);
		sstr>>buf1>>buf2;
		rs.insert(PSS(buf1, buf2));
	}
	fin.close();
	return 0;
}

int group::parse()
{
	for(int i = 0; i < items.size(); i++) items[i].parse(fs, rs);
	return 0;

	set<string> list;
	for(int i = 0; i < items.size(); i++) 
	{
		list.insert(items[i].words.begin(), items[i].words.end());
	}

	vector<string> vv(list.begin(), list.end());
	sort(vv.begin(), vv.end());

	for(int i = 0; i < vv.size() - 1; i++)
	{
		int d = item::distance(vv[i], vv[i + 1]);
		int l = item::lcs(vv[i], vv[i + 1]);

		string s1 = vv[i] + "s";
		string s2 = vv[i + 1] + "s";

		if(s1 == vv[i + 1]) printf("%s %s %d %d @\n", vv[i].c_str(), vv[i + 1].c_str(), d, l);
		else if(s2 == vv[i]) printf("%s %s %d %d @\n", vv[i].c_str(), vv[i + 1].c_str(), d, l);
		else printf("%s %s %d %d\n", vv[i].c_str(), vv[i + 1].c_str(), d, l);
	}
	return 0;
}

int group::compare()
{
	for(int i = 0; i < items.size(); i++) gr.add_vertex();

	for(int i = 0; i < items.size(); i++)
	{
		for(int j = i + 1; j < items.size(); j++)
		{
			double s = items[i].similarity(items[j]);
			if(s <= 0.8) continue;

			gr.add_edge(i, j);
			//printf("|%s| v.s. |%s| -> similarity = %.2lf\n", items[i].name.c_str(), items[j].name.c_str(), s);
		}
	}


	vector< set<int> > vv = gr.compute_connected_components();

	sort(vv.begin(), vv.end(), set_compare);

	for(int i = 0; i < vv.size(); i++)
	{
		set<int> &s = vv[i];
		for(set<int>::iterator it = s.begin(); it != s.end(); it++)
		{
			int k = (*it);
			printf("GROUP %d: ", i);
			items[k].print();
		}
		printf("\n");
	}
	return 0;
}

int group::print()
{
	for(int i = 0; i < items.size(); i++) 
	{
		items[i].print();
	}
	return 0;
}

bool set_compare(const set<int> &s1, const set<int> &s2)
{
	if(s1.size() > s2.size()) return true;
	else return false;
}
