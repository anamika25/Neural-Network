#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>

#include "Backprop.h"
#include "read.h"

using namespace std;

vector<string> split(string line, char delimiter)
{
	cout<<"in split";
	vector<string> res;
	stringstream s(line);
	string i;
	while( s.good() )
	{
	    if(getline(s, i, delimiter ))
	    res.push_back(i);
	}

	return res;
}

//store attribute
vector<attribute*> attributes(string filename)
{
	cout<<"in attribute";
	vector<string> full;
	vector<attribute*> attr;
	attribute *a = new attribute();
	ifstream infile(filename);
	string line;
	while (getline(infile, line))
	{
	     full=split(line,',');
	     if(full[1]=="continuous") a->type=true;
	     if(full[1]=="discrete") a->type=false;
	     attr.push_back(a);
	}
	return attr;
}

//store input data
vector<vector<string> > read_input_data(string filename,int& file_size, vector<attribute*>& attr)
{
	cout<<"in read_input_data";
	vector<vector<string> > data;
	//int limit=file_size*0.3;
	ifstream infile(filename);
	string line;
	vector<string> full;
	int i=0;
	float min=INT_MAX;
	float max=0.0;
	while (getline(infile, line))
	{
		full=split(line,',');
		data[i]=full;
		for(int j=0;j<full.size();j++)
		{
			if(min > stof(full[j])) min= stof(full[j]);
			if(max < stof(full[j])) max= stof(full[j]);
			attr[j]->min=min;
			attr[j]->max=max;
		}
		i++;
	}
	file_size=i;
	return data;
}

void count(vector<vector<string> > data,vector<attribute*>& attr)
{
	vector<string> t;
	int count=0;
	for(int i=0;i<attr.size()-1;i++)
	{
		for(int j=0;j<data.size();j++)
		{
			//if(t.find(data[j][i])==t.end())
			if(find(t.begin(),t.end(),data[j][i])==t.end())
			{
				t.push_back(data[j][i]);
				count++;
			}
		}
		attr[i]->count=count;
		t.clear();
		count=0;
	}
}
//normalize data values (0,1)
void Normalize(vector<vector<string> > data, vector<attribute*>& attr)
{
	cout<<"in normalize";
	float val;
	vector<string> vec;
	vector<string> target;
	int target_loc=attr.size()-1;
	for(int j=0;j<data.size();j++)
	{
		for(int i=0;i<attr.size()-1;i++)
			{
				vec=data[j];
				if(attr[i]->type==true)
				{
					//cont
					val=stof(vec[i]);
					val= (val- attr[i]->min)/(attr[i]->max - attr[i]->min);
					vec[i]=to_string(val);
				}
				else //discrete
				{
					val=stof(vec[i]);
					val= val/(attr[i]->count);
					vec[i]=to_string(val);
				}
			}
		int i= target_loc;
		if(attr[target_loc]->type==true)
			{
				//continuous target
				vec=data[j];
				val=stof(vec[i]);
				val= (val- attr[i]->min)/(attr[i]->max - attr[i]->min);
				vec[i]=to_string(val);
			}
			else
			{
				//discrete target
				vec=data[j];/*
				if(target.find(vec[i])==target.end())
				{
					target.push_back(vec[i]);
					count++;
				}*/
			}
	}
	//if(attr[target_loc]->type==false)
	//	target_nodes=count;

}
