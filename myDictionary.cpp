#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

map <string,string> dict;

// determine whether the word exist
bool isExist(string str){
	map<string,string>::iterator it;
	it =dict.find(str);
	if(it != dict.end()) return true;
	else
		return false;
}

// function for substring and mapping
string mapFromTxt(string &preline){
	int pos = preline.find(' ',0);        
	string key = preline.substr(0,pos);			 
	preline.erase(0,pos+1);
	return key; 
}

int main(){
	// read the file
	ifstream inFile("knownWords.txt");

	// get the first line to preline
	string line, preline;
	getline(inFile, preline);	
	while(getline(inFile, line)){					
		if(line[0]=='-'){			
			line.erase(0,1);
			preline += line;
		}		
		else{			   
			dict[mapFromTxt(preline)] = preline;
			preline = line;			
		}
	}
	// map the last remainder string
	dict[mapFromTxt(preline)] = preline;
	inFile.close();
	// file closed

	// command -> tasks 
	string cmd;
	while(getline(cin, cmd)){
		istringstream iss(cmd);
		string sub;
		iss >> sub;
		if(sub == "find"){
			map<string,string>::iterator it;
			string word1;
			iss >> word1;

			// find rank(index)
			if(isdigit(word1[0])){				
				char tmp[1024];
				strcpy(tmp, word1.c_str());
				int length = atoi(tmp),index;
				for(index = 1, it = dict.begin() ; it!=dict.end(); ++it, ++index){
					if(index == length){
						cout << it->first << endl;
						break;
					}
				}
				continue;
			}

			string word2 = "";
			iss >> word2;
			if(word2.length() ==  0 ){
				// find a word			
				if(!isExist(word1)) {
					continue;
				}
				cout << dict.find(word1)->second << endl;
			}
			else{
				for(map<string,string>::iterator it = dict.begin(); it != dict.end(); ++it){
					string word3 = it->first;
					if((word3 >= word1 && word3 <= word2) || (word3 >= word2 && word3 <= word1))
						cout << word3 << endl;
					else if(word3 > word2 && word3 > word1)
						break;
				}
			}
		}
		else if(sub == "delete"){
			map<string,string>::iterator it;
			string wordToDelete;
			iss >> wordToDelete;
			it = dict.find(wordToDelete);
			if(it != dict.end()) {
				dict.erase(it);
			}
		}
		else if(sub == "new"){
			string newWord,tmp;
			string newDef ="";			
			iss >> newWord;
			while(iss >> tmp)
				newDef = newDef + tmp + ' ';			
			dict[newWord] = newDef;
		}
		else if(sub == "count"){
			cout << dict.size() << endl;
		}
		else if(sub == "quit"){
			break;
		}
	}
	return 0;
}

