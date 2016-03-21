#include<iostream>
#include<vector>

using namespace std;

vector<string> split(string line, char delimiter);
vector<attribute*> attributes(string filename);
vector<vector<string> > read_input_data(string filename,int& file_size);
void Normalize(vector<vector<string> > data, vector<attribute*> attr);

