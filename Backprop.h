#include<iostream>
#include<vector>
#include<list>
#include<sstream>
#include<fstream>
#include<unordered_set>
using namespace std;

vector<float> form_target_vector(string target);
int in_curr(int layer);
vector<float> backprop(vector<float> input,vector<float> target,float& error,int col_no);
void build_net( int input,int num_hidden,int num_units,int output);
void Make_connection(int input,int output,int row, int col,int num_units);


class node
{
public:
	vector<float> wt;
	vector<float> tmp_wt;
	bool present;
	float data;
	node()
	{
		present=true;
		data=0.0;
	}
};



class attribute
{
public:
	bool type; //true for cont, false for disc
	float min;
	float max;
	int count; //get it by reading input of that column
	attribute()
	{
		type=false;
		min=INT_MAX;
		max=0.0;
		count=0;
	}
};

