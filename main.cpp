#include "headfile.h"    //Self-made headfile which includes all headfiles

using namespace std;

double calculation(double num1, double num2, char operation);
void calculate_in_loop(vector<double>& numbers, vector<char>& operation);
void calculate_in_endings(vector<double>& numbers, vector<char>& operation, int index);

int main() {
	cout << "Input your mathematical expression:\n"
		<< "(capable for + - * / ^ and () operations!)\n" << endl;
	string expression; cin >> expression;
	vector<char> operation;   //Initialize Stack for operations
	vector<double> numbers;   //Initialize Stack for numbers
	int precision = 0;
	for (auto element : expression) {
		if ('0' <= element && element <= '9') {                //Input Situation 1: Number
			if (operation.size() == 0) {
				numbers.push_back(static_cast<double>(element) - 48);
			}
			else {
				if (operation.back() == '.') {                  //Check if it's getting a decimal number
					precision++;
					numbers.back() += (static_cast<double>(element) - 48) / pow(10, precision);
				}
				else {
					numbers.push_back(static_cast<double>(element) - 48);
				}
			}
		}
		else {                      //Input Situation 2: Operation
			if (operation.size() != 0){
				if (operation.back() == '.') {    //If the previous number is a decimal, pop '.' outta stack "operation"
					operation.pop_back();
					precision = 0;    //Set precision to default (0)
				}
			}
			//The first priority: 3 operations that can push in without any consideration ('.', '(', and '^')
			if (element == '.' || element == '(' || element == '^') {
				operation.push_back(element);
			}
			else if (element == ')') {    //When ')' occurs, calculate until hit the nearest '('
				while (operation.back() != '(') {    //calculate until find the matched '('
					calculate_in_loop(numbers, operation);
				}
				operation.pop_back();    //Pop off '('
			}
			else if (element == '*' || element == '/') {    //The second priority: '*' and '/'
				if (operation.size() != 0) {   //Check if the operation stack is empty (to avoid range overflow)
					if (operation.back() == '*' || operation.back() == '/' || operation.back() == '^')
						calculate_in_loop(numbers, operation);
				}
				operation.push_back(element);
			}
			else {      //The third priority: '+' and '-'
				if (operation.size() != 0) {   //Check if the operation stack is empty (to avoid range overflow)
					//Everything has greater (or equal) priority than it so just calculate the privious operation
					if (operation.back() != '(') 
						calculate_in_loop(numbers, operation);
				}
				operation.push_back(element);
			}
		}
	}
	int index = operation.size() - 1;  //Initialize index to the end of the operation stack
	for (index; index >= 0; index--) {  //Traverse from right to left to find '^'
		if (operation[index] == '^')
			calculate_in_endings(numbers, operation, index);
	}
	index = 0;   //Set index to 0
	for (index; index < operation.size(); index++) {  //Traverse from left to right to find '*' or '/'
		if (operation[index] == '*' || operation[index] == '/') {
			calculate_in_endings(numbers, operation, index);
			index--;
		}
	}
	index = 0;   //Set index to 0
	while (operation.size() != 0) {  //Traverse from left to right to find '+' or '-'
		calculate_in_endings(numbers, operation, index);
	}
	//Calculation over! output the last number:
	cout << "The result is: " << numbers.back() << endl;
	return 0;
}

double calculation(double num1, double num2, char operation) {  //Fuction for basic calculations
	switch (operation) {
	case '+': return num1 + num2; break;
	case '-': return num1 - num2; break;
	case '*': return num1 * num2; break;
	case '/': return num1 / num2; break;
	case '^': return pow(num1, num2); break;
	default: break;
	}
}

//Function for manipulating stacks when calculating in main loop
void calculate_in_loop(vector<double>& numbers, vector<char>& operation) { 
	double assistNumber = calculation(numbers[numbers.size() - 2], numbers.back(), operation.back());
	numbers.pop_back(); numbers.pop_back();
	numbers.push_back(assistNumber);
	operation.pop_back();
}

//Function for manipulating stacks with iterator when calculating after traversal ends
void calculate_in_endings(vector<double>& numbers, vector<char>& operation, int index) {
	double assistNumber = calculation(numbers[index], numbers[index + 1], operation[index]);
	numbers.erase(numbers.begin() + index);
	numbers.erase(numbers.begin() + index);
	numbers.insert(numbers.begin() + index, assistNumber);
	operation.erase(operation.begin() + index);
}
