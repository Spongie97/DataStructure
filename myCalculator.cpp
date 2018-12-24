#include <iostream>
#include <stack>
#include <string>
#include <limits>
#include <cmath>
using namespace std;

void init();
bool syntaxError(string);
string toPostfix(string);
bool isDigit(char);
bool isOperator(char);
int precedence(char);
int postfixToAnswer(string);

bool illegalInput = 0;
bool overflow = 0;
bool underflow = 0;
string postfixExp;

int main(){
	string str;	
	while(getline(cin,str)){	
		// Initialize 
		init();

		// Check if there is a syntax error
		illegalInput = syntaxError(str);
		if(illegalInput){
			cout << "error" << endl;
			continue;
		}

		// Get the postfix expression
		postfixExp = toPostfix(str);
		// Calculate the answer
		int ans = postfixToAnswer(postfixExp);
		// If there is an undefined error (Ex: 1 / 0 ), display "Error"
		if(illegalInput){
			cout << "error\n";
			continue;
		}
		if(overflow){
			cout << "overflow\n";
			continue;
		}
		if(underflow){
			cout << "underflow\n";
			continue;
		}
		// Print the result
		// cout << "Postfix expression: " << postfixExp <<endl;
		cout << ans << endl;
	}
	return 0;
}

void init(){
	// true when the input is illegal
	illegalInput = false;
	overflow = 0;
	underflow = 0;
	// initialize postfixExp
	postfixExp = "";
}
/* If there is a syntax error, this function returns true */
bool syntaxError(string str){
	int left = 0, right = 0;
	// last one must not be an operator
	if(isOperator(str[str.length()-1]))	return true;
	for(int i = 0; i < str.length()-1; ++i){
		if(str[i] == '-' && isDigit(str[i+1]))
			str[i] = '_';
	}
	if(isOperator(str[0])) return true;
	for(int i = 0; i < str.length(); ++i){
		if(str[i] == '-' && isDigit(str[i+1]))
			str[i] = '_';
		if(str[i]!=' ') {			
			if(str[i] == '(') left++;
			if(str[i] == ')') right++;
			// more ) than (
			if(left < right) return true;
		}
		else {
			if( i + 1 < str.length() && i - 1 >= 0){
				// digit + digit Ex: 1 3 + 4
				if(isDigit(str[i-1]) && isDigit(str[i+1])) return true;
				// operator + operator Ex: 3 + + 5
				if(isOperator(str[i-1]) && isOperator(str[i+1]))return true;
				// not closed Ex: 1 + ) 
				if(isOperator(str[i-1]) && str[i+1]==')')return true;
				// not closed Ex: ( + 1
				if(str[i-1]=='(' && isOperator(str[i+1])) return true;
				// blank () 
				if(str[i-1]=='(' && str[i+1] == ')') return true;
				if(str[i+1] == '_' && !isDigit(str[i+2])) return true;
				if(str[i+1] == '_' && !isOperator(str[i-1])) return true;
			}
		}
	}
	// () not paired
	if(left != right) return true;
	// none of above error, return false
	return false;
}

/* turn infix into postfix */
string toPostfix(string str){
	// flag to record whether and integer input ends	
	bool flag = false;
	stack<char> aStack;
	// The following algorithm is based on the textbook
	for(int i = 0; i < str.length(); ++i){
		if(str[i]!=' '){
			if(isDigit(str[i])){
				postfixExp += str[i];
				flag = true; 
			}
			else{
				if(str[i] == '-' && isDigit(str[i+1]))
					str[i] = '_';
				if(str[i] == '(')
					aStack.push(str[i]);
				else if(str[i] == ')'){
					while(aStack.top()!='('){
						postfixExp += aStack.top();
						postfixExp += " ";
						aStack.pop();
					}
					aStack.pop();
				}
				else{
					while(!aStack.empty() && aStack.top()!='(' 
						&& precedence(str[i]) <= precedence(aStack.top())){
						postfixExp += aStack.top();
						postfixExp += " ";
						aStack.pop();
					}
					aStack.push(str[i]);
				}
			}
		}
		else if(flag){
			postfixExp += " ";
			flag = false;
		}
	}	
	// If the last char is not a blank, we add a blank
	if(postfixExp[postfixExp.length()-1]!=' ')
		postfixExp += " ";	
	// pop everything out of aStack
	while(!aStack.empty()){
		postfixExp += aStack.top();
		postfixExp += " ";
		aStack.pop();
	}
	// return the postfix Expression
	return postfixExp;
}

/* function that determines whether the char is a digit */
bool isDigit(char c){
	if (c >=48 && c<=57) return true;
	else
		return false;
}

/* function that determines whether the char is an operator */
bool isOperator(char c){
	if(c == '+' || c == '-' || c == '*' || c == '/' || c== '%')
		return true;
	else
		return false;
}

/* function that returns precedence of an operator */
int precedence(char c){
	switch(c){
		case '+':
			return 1;
		case '-':
			return 1;
		case '%':
			return 2;
		case '*':
			return 3;
		case '/':
			return 3;
		case '_':
		    return 4;
	}
}

/* function that calculates the answer */
int postfixToAnswer(string str){
	// stack num store the integers
	stack<long long int> num;
	// temp for calulating the value of integers
	long long int temp = 0;
	// flag for blank control
	bool flag = false;

	for(int i = 0 ; str[i]!='\0' ; ++i)
	{
		if(str[i]!=' '){
			// If its a digit, turn char into an integer
			if(isDigit(str[i])){
				temp = temp * 10 + str[i] - 48;
				flag = true;
			}
			else{
				// If its an operator, pop two integer from stack num
				// and do the math
				long long int a, b;
				b = num.top(); num.pop();
				if(str[i] == '_'){
					b = 0 - b;
					a = b;
				}
				else{
					a = num.top(); num.pop();
					switch(str[i])
					{
						case '+':
							a = a + b;
							break;
						case '-':
							a = a - b;
							break;
						case '*':
							a = a * b;
							break;
						case '/':
							if(b == 0){
								illegalInput = true;
								return -1;
							}
							a = a / b;
							break;
						case '%':
							if(b == 0){
								illegalInput = true;
								return -1;
							}					
							a = a % b;
							break;
					}
				}
				// After doing the math, push the result
				// into stack num
				num.push(a);
			}
		}
		else if(flag){
			// push the integers into num
			num.push(temp);
			flag = false;
			temp = 0;
		}
	}
	if(illegalInput)
		return -1;
	long long int ans =  num.top();
	num.pop();
	if(ans > pow(2,31) - 1){
		overflow = 1;
		return -1;
	}
	if(ans < (-pow(2, 31)) + 1){
		underflow = 1;
		return -1;
	}
	// return the calculated answer
	return ans;
}