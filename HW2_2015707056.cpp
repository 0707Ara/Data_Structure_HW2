//2015707056 조아라 ARA JO
#define _CRT_SECURE_NO_WARNINGS
#include <cctype>     // Provides isdigit
#include <cstdlib>    // Provides EXIT_SUCCESS
#include <cstring>    // Provides strchr
#include <iostream>   // Provides cout, cin, peek, ignore
#include <stack>      // Provides the stack template class
#include <queue>      // Provides the queue
#include <iomanip>    // Provides the setprecision

using namespace std;

void read_and_to_postfix(istream& ins);
void evaluate_postfix(stack<double>& results, queue<char>& postfix_operators, int& errorz);
bool HasHigherPrecedence(char op1, char op2);
bool IsLeftParentheses(char C);
bool IsRightParentheses(char C);
bool IsOperator(char C);
int GetOperatorWeight(char op);
void oppositeparentheses(char& c);


int main(void) {

	read_and_to_postfix(cin);
	return EXIT_SUCCESS;
}


void read_and_to_postfix(istream& ins)
{
	const char DECIMAL = '.';

	stack<double> numbers;
	stack<char> operators;
	stack<double> results;
	queue<char> postfix_operators;
	queue<double> final_results;
	queue<int> error_index;

	double number;
	char symbol;
	char s[100];
	char init[4];
	strcpy(init, "EOI");
	int order_of_line = 1;
	int num_of_parentheses = 0;
	int errorz = 0;
	int errorp = 0;

	while (ins)
	{
		if (isdigit(ins.peek()) || (ins.peek() == DECIMAL)) {
			//If the input stream is number or start with the DECIMAL read the input and put it in the stack named result
			ins >> number;
			results.push(number);
		}
		else if (IsOperator(ins.peek())) {
			//If the input stream value is operator go through while loop 
			//until the operator stack is empty and the current operator has lower precedence
			//If the operator has higher or same precedence put the operator to the stack named posfix_operator and do the postfix calculation 
			ins >> symbol;
			while (!operators.empty() && !IsLeftParentheses(operators.top()) && HasHigherPrecedence(operators.top(), symbol)) {
				postfix_operators.push(operators.top());
				evaluate_postfix(results, postfix_operators, errorz);
				operators.pop();
			}operators.push(symbol);
		}
		else if (IsLeftParentheses(ins.peek())) {
			//If the input stream is one of left parentheses push it in the operators 
			//and count the number of parentheses to check the total input parentheses is balanced
			ins >> symbol;
			operators.push(symbol);
			num_of_parentheses++;
		}
		else if (IsRightParentheses(ins.peek())) {
			//If the input stream is one of right parentheses first search for matching left parentheses.
			//If there is matching left parentheses, pop it and break the loop.
			//If there is no matching but still left parentheses it is parentheses error. 
			//Increase the errorp which counts the error of parenthese.
			//While poping the operators untill find the lest parentheses, and the stack operator gets empty it is also parentheses error.
			ins >> symbol;
			num_of_parentheses--;
			oppositeparentheses(symbol);

			while (!operators.empty()) {
				if (operators.top() == symbol) {
					operators.pop();
					break;
				}

				if (IsOperator(operators.top())) {
					postfix_operators.push(operators.top());
					operators.pop();
					evaluate_postfix(results, postfix_operators, errorz);
				}
				else if (IsLeftParentheses(operators.top())) {
					operators.pop();
					errorp++;
				}

				if (operators.empty()) {
					errorp++;
					break;
				}


			}
		}
		else if (ins.peek() == '\n') {
			//If the enter is typed, ignore the current input and push all the oprators into postfix oprator stack.
			//Then calculate the postfix expression
			//The final result will be on the top of the stack result
			//Check first if there were parentheses error or divided by 0 error by looking at errorp,errorz index.
			//If there were errors puth the 0 or 1 to final results and push the error line into queue error index
			ins.ignore();
			while (!operators.empty() && IsOperator(operators.top())) {
				postfix_operators.push(operators.top());
				operators.pop();
			}
			evaluate_postfix(results, postfix_operators, errorz);
			if (num_of_parentheses != 0)
				errorp++;
			//This will discern if there is error or not.
			if (errorz == 0 && errorp == 0) {
				final_results.push(results.top());
				final_results.front();
			}
			else if (errorz != 0) {
				final_results.push(0);
				error_index.push(order_of_line);
				errorz = 0;
			}
			else if (errorp != 0) {
				final_results.push(1);
				error_index.push(order_of_line);
				errorp = 0;
				num_of_parentheses = 0;
			}

			//This clears the stack and queue that has used.
			while (!results.empty()) {
				results.pop();
			}
			while (!operators.empty()) {
				operators.pop();
			}
			while (!postfix_operators.empty()) {
				postfix_operators.pop();
			}

			order_of_line++;
		}

		else if (ins.peek() == 'E' || ins.peek() == 'O' || ins.peek() == 'I') {
			//If "EOI" typed straight then show all the result in the queue final_results
			if (ins.peek() == 'E') //If 'E' has typed, clear the array to compare with new inputs
				for (int i = 0; i < sizeof(s); i++)
					s[i] = 0;
			ins >> s;
			if (strcmp(s, init) == 0) {
				int i = 1;
				while (!final_results.empty()) {

					if (!error_index.empty()) {
						if (i == error_index.front() && final_results.front() == 0) {
							cout << "Error!: divided by zero" << endl;
							error_index.pop();

						}
						else if (i == error_index.front() && final_results.front() == 1) {
							cout << "Error!: unbalanced parentheses" << endl;
							error_index.pop();

						}
						else
							cout << fixed << setprecision(3) << final_results.front() << endl;
						final_results.pop(); i++;
						continue;
					}
					cout << fixed << setprecision(3) << final_results.front() << endl;

					final_results.pop(); i++;
				}
				return;
			}
		}
		else
			ins.ignore();
	}
}

//This function evaluates the postfix expression whenever the oprator is pushed 
void evaluate_postfix(stack<double>& results, queue<char>& postfix_operators, int& errorz)
{
	double operand1, operand2;
	char symbol;

	while (!postfix_operators.empty() && !results.empty()) {

		operand2 = results.top(); results.pop();
		operand1 = results.top(); results.pop();
		symbol = postfix_operators.front(); postfix_operators.pop();

		if (symbol == '+')
			results.push(operand1 + operand2);
		else if (symbol == '-')
			results.push(operand1 - operand2);
		else if (symbol == '*')
			results.push(operand1 * operand2);
		else if (symbol == '/') {
			if (operand2 == 0) {
				errorz++;
				results.push(0);
			}
			else
				results.push(operand1 / operand2);
		}
	}

}

//The HasHigherPrecedence function return the oprator has higher precedence or not by comparing the weight of the two input
bool HasHigherPrecedence(char op1, char op2)
{
	int op1Weight = GetOperatorWeight(op1);
	int op2Weight = GetOperatorWeight(op2);
	return (op1Weight >= op2Weight);
}

//The GetOperatorWeight function return the weight of the current operator.
int GetOperatorWeight(char op)
{
	int weight = 0;
	switch (op)
	{
	case '+':
	case '-':
		weight = 1;
		break;
	case '*':
	case '/':
		weight = 2;
		break;
	}
	return weight;
}

//The IsLeftParentheses function checks if the input is one of the left parentheses.
bool IsLeftParentheses(char C) {
	return (C == '(' || C == '{' || C == '[');
}

//The IsRightParentheses function checks if the input is one of the right parentheses.
bool IsRightParentheses(char C) {
	return (C == ')' || C == '}' || C == ']');
}

//The IsOperator function checks if the input is one of the operators(+-*/)
bool IsOperator(char C) {
	return (C == '+' || C == '-' || C == '*' || C == '/');
}

//The oppositeparentheses function returns the opposite parentheses. 
void oppositeparentheses(char& c) {

	if (c == ')')
		c = '(';
	else if (c == ']')
		c = '[';
	else if (c == '}')
		c = '{';
}

