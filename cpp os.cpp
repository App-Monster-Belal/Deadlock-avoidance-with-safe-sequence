#include <iostream>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>
using namespace std;
int nprocesses = 5, nresources = 4;
vector<int> seq;

typedef struct {
	int id;
	vector<int> Max;			// 2D array Max[i,1] = k, means pi need max k instances of resource 1
	vector<int> Allocation;	// 2D array Allocation[i,1] = k, means pi has k instances of resource 1
	vector<int> Need;			// 2D array Need[i,1] = k, means pi need k instances of resource 1 to start exec
	bool status;
}process;


bool IsSafe(vector<process>  processes, vector<int> available){
	
	int finished = nprocesses;
	int j =0;
	while(finished != 0){
		int counter = finished;
		for(auto i = processes.begin(); i != processes.end(); i++){
			if((*i).status == false ){
				for( j = 0; j<nresources; j++)
					if ((*i).Need[j] > available[j]) break;
				if(j == nresources){
					finished --;
					(*i).status = true;
					for(int k =0; k<nresources; k++)
						available[k] += (*i).Allocation[k];
					seq.push_back((*i).id);
					
				}
			}
		}
		if(counter == finished){
			return false;
		}
		
	}
	return true;
	
	/*
	auto i = processes.begin();
	unsigned int ind= 0;
	int j = 0;
	while(!processes.empty() && i!=processes.end()){
		for( j = 0; j<nresources; j++)
			if ((*i).Need[j] > available[j]) break;
		//cout<<j<<endl;
		if(j == nresources){
			for(int k =0; k<nresources; k++)
				available[k] += (*i).Allocation[k];
			seq.push_back((*i).id);
			ind = i - processes.begin() + 1;
			if(ind ==processes.size()){
				processes.erase(i);
				i = processes.begin();
			}
			else{
				processes.erase(i);
				i = processes.begin()+(ind-1);

			}

		}
		else i++;
	}
	if(processes.empty())
		return true;
	else return false;
	*/
}

bool Requset(vector<process>  processes, vector<int> available, int p , vector<int> req){
	int j = 0;
	for( j = 0; j<nresources; j++)
		if (req[j] > available[j] || req[j] > processes[p].Need[j]) break;
	if(j!=nresources) return false;

	for( j = 0; j<nresources; j++){
		processes[p].Allocation[j] += req[j];
		processes[p].Need[j] -= req[j];
		available[j] -= req[j];
	}
	// Print Need Matrix
	cout<<"\n\tNeed Matrix";
	for(int j = -1; j < nprocesses; j++){
		cout<<endl;
		if(j==-1){
			cout<<"  \t";
			for(int i = 0; i < nresources; i++){
				cout<<"R"<<i<<"\t";
			}
			continue;
		}
		cout<<"P"<<j<<"\t";
		for(int i = 0; i < nresources; i++)
			cout<<processes[j].Need[i]<<"\t";
	}
	cout<<endl<<endl;
	return IsSafe(processes, available);

}

int main(){
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	vector<int> available(nresources);				// num of instances for each resource
	vector<process> processes(nprocesses);
	vector<int> req(nresources,0);
	int choice;
	int p;
	char sysexit;
	do{
		// clear vectors--
		seq.clear();
		available.clear();
		processes.clear();
		available.resize(nresources);
		processes.resize(nprocesses);
		
		
		cout<<"\nEnter num of resources : ";
		cin>>nresources;
		cout<<"Enter num of processes : ";
		cin>>nprocesses;
		cout<<endl;
		
		// Get Allocation Matrix
		for(int i = 0; i < nprocesses; i++)
		{
			processes[i].Max.resize(nresources);
			processes[i].Allocation.resize(nresources);
			processes[i].Need.resize(nresources);
			processes[i].id = i;
			processes[i].status= false;

			cout<< "Enter Allocation array for P"<< i<<" : ";
			for(int j = 0; j < nresources; j++){
				cin>>processes[i].Allocation[j] ;
			}
		}
		cout<<endl;
		
		// Get Max Matrix
		for(int i = 0; i < nprocesses; i++)
		{
			cout<< "Enter Max array for P"<< i<<" : ";
			for(int j = 0; j < nresources; j++){
				cin>> processes[i].Max[j] ;
				processes[i].Need[j] = processes[i].Max[j] - processes[i].Allocation[j];
			}
		}
		
		
		// Get Available Matrix
		cout<< "\nEnter Available array  "<<endl;
		for(int j = 0; j < nresources; j++)
			cin>>available[j];
		
		
		// Print Need Matrix
		cout<<"\n\tNeed Matrix";
		for(int j = -1; j < nprocesses; j++){
			cout<<endl;
			if(j==-1){
				cout<<"  \t";
				for(int i = 0; i < nresources; i++){
					cout<<"R"<<i<<"\t";
				}
				continue;
			}
			cout<<"P"<<j<<"\t";
			for(int i = 0; i < nresources; i++)
				cout<<processes[j].Need[i]<<"\t";
		}
		cout<<endl;


		
		cout<<"\nDo you want to check safety? (1-yes 2-no)\n";
		cout<< "your choice : ";
		cin>>choice;
		if(choice == 1){
			if(IsSafe(processes, available)){
				cout<<"Yes , Safe state <";
				for(int i = 0; i<nprocesses; i++){
					
					cout<<"P"<<seq[i];
					if(i != nprocesses-1) cout<<",";
					else cout<<">";
				}
			}
			else{
				cout<<"No";
			}
		}
		cout<<endl;
		
		req.clear();
		req.resize(nresources,0);
		cout<<"\nDo you want to make request? (1-yes 2-no)\n";
		cout<< "your choice : ";
		cin>>choice;
		if(choice == 1){
				cout<<"enter index of process: ";
				cin>>p;
				cout<<"enter request of process "<<p<<" : ";
				for(int i = 0; i<nresources; i++)
					cin>>req[i];
				if(Requset(processes, available, p, req)){
					cout<<"Yes , Safe state <";
					cout<<"P"<<p<<"req,";
					for(int i = 0; i<nprocesses; i++){
						cout<<"P"<<seq[i];
						/*
						if(seq[i]==p) cout<<"P"<<seq[i]<<"req";
						else  cout<<"P"<<seq[i];
						*/
						if(i != nprocesses-1) cout<<",";
						else cout<<">";
					}
				}
				else{
					cout<<"No";
				}
		}
		
		
	
		cout<<"\n\nDo you want to test agian?(y/n)\n";
		cin>>sysexit;
	}while(sysexit=='y'||sysexit=='Y');



	return 0;
}
