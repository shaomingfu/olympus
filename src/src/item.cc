#include "item.h"
#include <cstdlib>
#include <vector>
#include <sstream>

item::item()
{
	value = 0;
}

bool item::build(const string &s)
{
	int p = s.find('\t');
	if(p == string::npos) return false;
	name = s.substr(0, p);
	string v = s.substr(p + 1, s.size() - p - 1);
	value = atof(v.c_str());
	return true;
}

int item::parse(const set<string> &fs, MSS &rs)
{
	string ss;
	for(int i = 0; i < name.size(); i++)
	{
		char c = name[i];
		if(c >= 'A' && c <= 'Z') ss.push_back(c + 32);
		else if(c >= 'a' && c <= 'z') ss.push_back(c);
		else if(c >= '0' && c <= '9') ss.push_back(c);
		else if(c == '&') ss.append(" and ");
		else if(c == '.') ss.push_back('.');
		else if(c == '\'') ss.push_back('\'');
		else if(c == '#') ss.push_back('#');
		else if(c == '*') ss.push_back(' ');
		else if(c == '+') ss.push_back(' ');
		else if(c == ':') ss.push_back(' ');
		else if(c == '_') ss.push_back(' ');
		else if(c == '|') ss.push_back(' ');
		else if(c == '\"') ss.push_back(' ');
		else if(c == '?') ss.push_back(' ');
		else if(c == '-') ss.push_back(' ');
		else if(c == '/') ss.push_back(' ');
		else if(c == '\\') ss.push_back(' ');
		else if(c == ' ') ss.push_back(' ');
		else if(c == '(') ss.push_back(' ');
		else if(c == ')') ss.push_back(' ');
		else if(c == ',') ss.push_back(' ');
		else ss.push_back(c);
	}

	//printf("|%s| -> |%s|\n", name.c_str(), ss.c_str());

	words.clear();
	stringstream sstr(ss);
	char line[10240];
	while(sstr>>line)
	{
		string s(line);
		if(fs.find(s) != fs.end()) continue;

		//if(rs.find(s) != rs.end()) words.push_back(rs[s]);	
		//else 
		words.push_back(s);
	}

	//printf("|%s| -> ", name.c_str());
	//for(int i = 0; i < words.size(); i++) printf("|%s|\n", words[i].c_str());

	return 0;
}

int item::print()
{
	printf("|%s|%d|\n", name.c_str(), value);
	return 0;
}

double item::similarity(const item &e)
{
	int m = words.size();
	int n = e.words.size();
	vector< vector<double> > vv;
	vv.resize(m);
	for(int i = 0; i < m; i++) vv[i].resize(n);
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			vv[i][j] = similarity(words[i], e.words[j]);
			//printf("%s %s -> %lf\n", words[i].c_str(), e.words[j].c_str(), vv[i][j]);
		}
	}

	set<int> s1;
	set<int> s2;
	
	double sm = 0;
	while(true)
	{
		int ki = -1;
		int kj = -1;
		double x = 0;
		for(int i = 0; i < m; i++)
		{
			if(s1.find(i) != s1.end()) continue;
			for(int j = 0; j < n; j++)
			{
				if(s2.find(j) != s2.end()) continue;
				if(vv[i][j] < x) continue;
				x = vv[i][j];
				ki = i;
				kj = j;
			}
		}
		if(ki == -1 || kj == -1) break;

		sm += x;
		s1.insert(ki);
		s2.insert(kj);
	}

	double ss = sm / (m > n ? m : n);
	//if(ss > 0.1) printf("|%s| <-> |%s| -> %.6lf\n", name.c_str(), e.name.c_str(), ss);
	return ss;
}

double item::similarity(const string &s1, const string &s2)
{
	if(s1 == s2) return 1.0;
	int l = s1.size() > s2.size() ? s1.size() : s2.size();
	return lcs(s1, s2) * 1.0 / l;
}

int item::distance(const string &s1, const string &s2)
{
	int m = s1.size() + 1;
	int n = s2.size() + 1;
	vector< vector<int> > table;
	table.resize(m);
	for(int i = 0; i < m; i++) table[i].resize(n);
	table[0][0] = 0;
	for(int i = 1; i < m; i++) table[i][0] = i;
	for(int j = 1; j < n; j++) table[0][j] = j;
	for(int i = 1; i < m; i++)
	{
		for(int j = 1; j < n; j++)
		{
			int d1 = table[i - 1][j] + 1;
			int d2 = table[i][j - 1] + 1;
			int d3 = table[i - 1][j - 1];
			if(s1[i - 1] != s2[j - 1]) d3 += 3;
			table[i][j] = (d1 <= d2 && d1 <= d3) ? d1 : (d2 <= d3 ? d2 : d3);
		}
	}
	return table[m - 1][n - 1];
}

int item::lcs(const string &s1, const string &s2)
{
	for(int i = 0; i < s1.size() && i < s2.size(); i++)
	{
		if(s1[i] != s2[i]) return i;
	}
	return s1.size() < s2.size() ? s1.size() : s2.size();
}
