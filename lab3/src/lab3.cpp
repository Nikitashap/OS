#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include <ctime>

using namespace std;


void experement_iteration (int sum1, int sum2, int k, int round, int &win1, int &win2)
{
	int sum1_exp=sum1;
	int sum2_exp=sum2;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1,6);  
	int j=0;
	while(j<k-round){
		sum1_exp+=dist(gen);
		sum1_exp+=dist(gen);
		sum2_exp+=dist(gen);
		sum2_exp+=dist(gen);
		j++;
	}
	if(sum1_exp>sum2_exp){
		win1++;
	}
	if(sum2_exp>sum1_exp){
		win2++;
	}
}
int min(int a, int b){
	if(a>=b){
		return b;
	} else{
		return a;
	}
}


int main(int argc, char *argv[]){
    int thread_number;
    if (argc < 2)
    {
        cout << "Enter thread amount:\n";
        cin >> thread_number;
    }
    else
    {
        thread_number = stoi(argv[1]);
    }
	int k,round,sum1,sum2,exp_num,win1=0,win2=0;
	cin >> k >> round >> sum1 >> sum2 >> exp_num;
	vector<thread> th(thread_number);
	unsigned int clock_start=clock();
	for(int i=0;i<exp_num;i+=thread_number){
		for(int j=0;j<min(exp_num-i,thread_number);j++){
			th[j] = thread(experement_iteration,ref(sum1),ref(sum2),ref(k),ref(round),ref(win1),ref(win2));
		}
		for(int j=0;j<min(exp_num-i,thread_number);j++){
			th[j].join();
		}
	}
	unsigned int clock_end=clock();
	cout << clock_end-clock_start << '\n';
	cout << "1 player chance to win is " << float(win1) / float(exp_num) << '\n';
	cout << "2 player chance to win is " << float(win2) / float(exp_num) << '\n';

}
