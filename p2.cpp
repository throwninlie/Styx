#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <utility>
#include <string>
//#include <set>

///dfas/dfweg/awe/rw/g/wer/asgasljgawogwje

//w/erwegaksdfkklslssllslsslls
//#include <queue>
///SOME RANDOM CRAP IN YOUR CODE
//hAAAAAAANDS
//HANDS WRITING WORDS
using namespace std;
int main(){
	int N,M,D;
	cin >> N >> M >> D;
	map<string,char> mymap;
	//cout << N << endl;
	//cout << M << endl;
	//cout << D << endl;
	for(int j = 0; j < M; j++){
		//vector<int> digits;
		//value is the credit card #
		char value[25];
		cin >> value;
		//lolz

		//sdfasdflkaweugow


		//sdgaiowueraowiudg



		//stuff
		for(int k = 0; k < D; k++){
			int d;
			char di = value[k];
			d = di - '0';
			//cout << "digit:" << d<<endl;
			int hash;
			hash = ((d + pow(k,4) + pow(D-k,4)) * pow(D,2)) / pow(D-3,2);
			string hashS;
			hashS = to_string(hash);
			//cout << "hash: " << hashS << endl;
			//cout << "digit: " << digits[k]<<endl;
			mymap.insert(mymap.begin(),pair<string,char>(hashS,di));
		}
	}
	//WORDS ASD
	//ASD;;LFKFJHAS;LFJ
	vector<string> creditlist;
	for(int l = 0; l < N; l++){
		//retrieve hashes here, input the different
		//hashes into array first
		vector<string> hashlist;
		char credit[25];
		for(int a = 0; a < D; a++){
			string hash;
			cin >> hash;

			//hashS = to_string(hash);
			hashlist.insert(hashlist.begin()+a,hash);
		}
		for(int p = 0; p < D; p++){
			//retrieve hash, store in array (don't print until end)
			string hash;
			hash = hashlist[p];
			//cout << "our hash is:" <<hash <<endl;
			char digit;
			map<string,char>::iterator it = mymap.begin();
			for(it = mymap.begin(); it!=mymap.end();++it){
				if (hash.compare(it->first) == 0){
					digit = it->second;
				}
			}
			//cout << "digit: " <<digit << endl;
			//cout << "power: " << pow(10,D-p-1)<<endl;
			//credit = credit + digit*pow(10,D-p-1);
			credit[p] = digit;
		}
		//cout << credit << endl;
		//string creditString;
		//creditString = to_string(credit);
		//add null character to end
		credit[D] = '\0';
		creditlist.push_back(credit);
	}
	for(int o = 0; o < N; o++){
		if( o == N-1){
			cout << creditlist[o];
		}
		else{
			cout << creditlist[o] <<endl;
		}
	}
	return 0;
}
