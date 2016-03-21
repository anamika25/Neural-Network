#include "Backprop.h"
#include "read.h"
#include<iostream>
#include<vector>
#include<math.h>

using namespace std;
vector<vector<node*> > network;
int is_target_cont;

// keep all the values if target type is discrete // fill when you read data
vector<string> discrete_target;

vector<float> bias; // size of this should be colm -1;


vector<float> form_target_vector(string target)
{
	vector<float> res(discrete_target.size(),0.0);
	int assn=0;
	for(int i=0;i<discrete_target.size();i++)
	{
		if(discrete_target[i].compare(target)==0)
		{
			assn=i;
			break;
		}
	}

	// now assign 1 for corresponding target and  0 for remaining

	for(int i=0;i<res.size();i++)
	{
		if(i==assn)
		{
			res[i]=1.0;
			break;
		}
	}

	return res;

}
int in_curr(int layer)
{
	int ret=0;
	for(int i=0;i<network.size();i++)
	{
		if(network[i][layer]->present)
		{
			ret++;
		}

	}
	return ret;
}
vector<float> backprop(vector<float> input,vector<float> target,float& error,int col_no)
{
	cout<<"in backprop";
	if(network.size()==0) return vector<float>();

	int next_size=network[0][col_no]->wt.size();
	vector<float> next_layer(next_size);

	if(col_no==network[0].size()-1)
	{
		// reached end node
		// compute error and do back pass
		for(int i=0;i<target.size();i++)
		{
			float tp = target[i]-input[i];
			error+=pow(tp,2)/2;
		}
		//return vector<float>();
	}

	//net value calculated by W*I
	for(int i=0;i<next_size;i++)
	{
		next_layer[i]=bias[col_no]*1.0;
		for(int j=0;j<network.size();j++) //check curr size not network size
		{
			if(network[j][col_no]->present)
			// compute fowrd pass output and assign it to next_layer[i] =
			next_layer[i]+= input[j]*(network[j][col_no]->wt[i]);
		}
		// add bias value to it
		// next_layer[i]+=bias[node_no]*1.0;
	}

	//out value after applying net value to function
	for(int i=0;i<next_layer.size();i++)
	{
		double x= next_layer[i];
		next_layer[i]= 1/(1+exp(-x));
	}


	vector<float> res=backprop(next_layer,target,error,col_no+1);

	// back pass

	int rows = in_curr(col_no);
	vector<float> return_input(rows);

	if(col_no==network.size()-2)
	{
		// output - hidden
		for(int i=0;i<next_size;i++)
		{
			for(int j=0;j<network.size();j++)
			{
				// compuation for weight update
				float inter = (target[i]-next_layer[j])*next_layer[j]*(1-next_layer[j])*input[j];
				// weight update
				// tmp_weight = weight - learning*inter*network[j][layer]->wt[j];
				network[j][col_no]->tmp_wt[j]=network[j][col_no]->wt[j]-0.4*inter;
				return_input[i]+=inter;
			}
		}

		return return_input;
	}
	else
	{
		// hidden to hidden

		for(int i=0;i<next_size;i++)
		{
			for(int j=0;j<network.size();j++)
			{
				// update computation for this one
				float inter = (target[i]-next_layer[j])*next_layer[j]*(1-next_layer[j])*input[j];
				network[j][col_no]->tmp_wt[j]=network[j][col_no]->wt[j]-0.4*inter*network[j][col_no]->wt[j];
				return_input[i]+=inter;
			}
		}

		return return_input;
	}

}
void build_net( int input,int num_hidden,int num_units,int output)
{
	cout<<"in build_net";
	string filename="xor_control.txt";
	int filesize;
	vector<int> count;
	vector<vector<string> > data=read_input_data("xor_data.txt",filesize);
	vector<attribute*> attr;

	int row,col;
	row= (input>num_units)? input : num_units;
	col= num_hidden+2;

	network.resize(row);
	for(int i=0;i<col-1;i++)
	{
		float b=rand()%1;
		bias.push_back(b);
	}

	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			node *a = new node();
			network[i].push_back(a);
			//anything else?
		}
	}

	attr= attributes(filename);
	Normalize(data,attr);
	Make_connection(input,output,row,col,num_units);

	// backprop
	vector<float> target;
	float error=0;
	int node_no=0;
	vector<string> value_s;
	vector<float> value_f;
	for(vector<string> value : data)
	{
		string each_target=value[value.size()-1];
		value.erase(value.begin()+value.size()-1);
		if(attr[value.size()-1]->type==true) is_target_cont=1;
		if(is_target_cont==0)
		target=form_target_vector(each_target);
		else
		{
			target.push_back(stof(each_target));
		}
		value_s=value;
	}

	for(int i=0;i<value_s.size();i++)
	{
		float f=stof(value_s[i]);
		value_f.push_back(f);
	}
		vector<float> res =backprop(value_f,target,error,node_no);
		for(int i=0;i<res.size();i++)
			{
				cout<<res[i]<<endl;
			}
		if(error<0.00001)
		{
			// stoping criteria for back prop
			exit(-1);
		}


}
/*
void feedforward(vector<vector<Node*> >& network, int row, int col,int num_units)
{
	float bias= rand()%1;
	//vector<float> hid;
	float neth,outh,val=bias;
	for(int j=0;j<num_units;j++)
	{
		for(int i=0;i<row;i++)
		{
			val+=((network[i][col].data)*(network[i][col].wt[j].second));
		}
		outh=1/(1+exp(-val));
		network[j][col].data=outh;
		val=0.0;
	}

}
*/
void Make_connection(int input,int output,int row, int col,int num_units)
{
	cout<<"in Make_connection";
	//input to hidden
	for(int i=0;i<input;i++)
	{
		for(int j=0;j<num_units;j++)
		{
			if(network[i][0]->present==true)
			{
				float weight=rand()%1;
				network[i][0]->wt.push_back(weight);
			}
		}
	}
	//hidden to hidden
	int c=1;
	while(c< (col-2))
	{
		for(int i=0;i<num_units;i++)
		{
			if(network[i][c]->present==true)
			{
				float weight=rand()%1;
				network[i][c]->wt.push_back(weight);
			}
		}
	}

	//hidden to output
	for(int i=0;i<num_units;i++)
		{
			for(int j=0;j<output;j++)
			{
				if(network[i][col-1]->present==true)
				{
					float weight=rand()%1;
					network[i][col-1]->wt.push_back(weight);
				}
			}
		}
}

vector<float> backprop_test(vector<float> input,int target,float& error,int col_no)
{
	cout<<"in backprop";
	//if(network.size()==0) return vector<float>();

	int next_size=2;
	vector<float> next_layer(2);

	if(col_no==2)
	{
		// reached end node
		// compute error and do back pass
			float tp = target-input[0];
			tp+=target-input[1];
			error+=pow(tp,2)/2;
		//return vector<float>();
	}

	//net value calculated by W*I
	for(int i=0;i<2;i++)
	{
		next_layer[i]=bias[col_no]*1.0;
		for(int j=0;j<network.size();j++) //check curr size not network size
		{
			if(network[j][col_no]->present)
			// compute fowrd pass output and assign it to next_layer[i] =
			next_layer[i]+= input[j]*(network[j][col_no]->wt[i]);
		}
		// add bias value to it
		// next_layer[i]+=bias[node_no]*1.0;
	}

	//out value after applying net value to function
	for(int i=0;i<next_layer.size();i++)
	{
		double x= next_layer[i];
		next_layer[i]= 1/(1+exp(-x));
	}


	//vector<float> res=backprop(next_layer,target,error,col_no+1);

	// back pass

	int rows = in_curr(col_no);
	vector<float> return_input(rows);

	if(col_no==network.size()-2)
	{
		// output - hidden
		for(int i=0;i<next_size;i++)
		{
			for(int j=0;j<network.size();j++)
			{
				// compuation for weight update
				float inter = (target-next_layer[j])*next_layer[j]*(1-next_layer[j])*input[j];
				// weight update
				// tmp_weight = weight - learning*inter*network[j][layer]->wt[j];
				network[j][col_no]->tmp_wt[j]=network[j][col_no]->wt[j]-0.4*inter;
				return_input[i]+=inter;
			}
		}

		return return_input;
	}
	else
	{
		// hidden to hidden

		for(int i=0;i<next_size;i++)
		{
			for(int j=0;j<network.size();j++)
			{
				// update computation for this one
				float inter = (target-next_layer[j])*next_layer[j]*(1-next_layer[j])*input[j];
				network[j][col_no]->tmp_wt[j]=network[j][col_no]->wt[j]-0.4*inter*network[j][col_no]->wt[j];
				return_input[i]+=inter;
			}
		}

		return return_input;
	}

}
int main()
{
	//int input=2,num_hidden=1,num_units=2,output=1; //pass by argv
	//build_net(input,num_hidden,num_units,output);
	vector<float> wt;
	network.resize(2);
		for(int i=0;i<2;i++)
		{
			float b=rand()%1;
			bias.push_back(b);
		}

		for(int i=0;i<6;i++)
		{
			float b=rand()%1;
			wt.push_back(b);
		}

		vector<float> ip;
		vector<float> op;
		float err=0.0;
		network[0][0]->data=1.0;
		network[0][0]->present=true;
		network[1][0]->data=0.0;
		network[1][0]->present=true;
		ip.push_back(1.0);
		ip.push_back(0.0);

		network[0][0]->wt.push_back(wt[0]);
		network[0][0]->wt.push_back(wt[1]);
		network[1][0]->wt.push_back(wt[2]);
		network[1][0]->wt.push_back(wt[3]);


		network[0][1]->wt.push_back(wt[4]);
		network[1][1]->wt.push_back(wt[5]);

		op=backprop_test(ip,1,err,0);
		for(int i=0;i<op.size();i++)
			cout<<op[i]<<endl;;

	return 0;
}
