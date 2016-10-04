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

int group::clust()
{
	vector<int> v;
	for(int i = 0; i < items.size(); i++) v.push_back(i);
	clust(v, 0, "GROUP");
	return 0;
}

int group::clust(const vector<int> &v, int level, string tag)
{
	if(level >= 4)
	{
		for(int k = 0; k < v.size(); k++)
		{
			printf("%s: ", tag.c_str());
			for(int i = 0; i < 20 - tag.size(); i++) printf(" ");
			items[v[k]].print();
		}
		return 0;
	}

	//printf("clust level %d with %lu elements\n", level, v.size());
	undirected_graph ug;
	for(int i = 0; i < v.size(); i++) ug.add_vertex();

	for(int i = 0; i < v.size(); i++)
	{
		item &e1 = items[v[i]];
		for(int j = i + 1; j < v.size(); j++)
		{
			item &e2 = items[v[j]];
			double s = e1.similarity(e2, level);
			if(s < 0.9) continue;
			ug.add_edge(i, j);
			//printf("|%s| v.s. |%s| -> similarity = %.2lf\n", items[i].name.c_str(), items[j].name.c_str(), s);
		}
	}

	vector< set<int> > vv = ug.compute_connected_components();

	sort(vv.begin(), vv.end(), set_compare);


	level++;

	for(int i = 0; i < vv.size(); i++)
	{
		vector<int> v1(vv[i].begin(), vv[i].end());
		vector<int> v2;
		//printf("process %d component with %lu elements\n", i, v1.size());
		for(int k = 0; k < v1.size(); k++)
		{
			v2.push_back(v[v1[k]]);
		}
		char buf[10240];
		sprintf(buf, "%s.%d", tag.c_str(), i + 1);
		clust(v2, level, buf);
		if(level == 2) printf("\n");
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
