#include <iostream>
#include <cmath>
#include <cstring>

/*******************************************************************
** Function: clamp
** Description: limits an int between 2 limits
** Parameters: x, low, high
** Pre-conditions: takes an int and 2 limits
** Post-conditions: limits an int between 2 limits
*******************************************************************/

int clamp(int x, int low, int high){
	if(x < low) return low;
	else if(x > high) return high;
	else return x;
}

/*******************************************************************
** Function: isLetter
** Description: check if a char is a letter
** Parameters: a
** Pre-conditions: takes 1 char
** Post-conditions: check if a char is a letter
*******************************************************************/

bool isLetter(char a){
	return (a>=65 && a<=90)||(a>=97 && a<=122);
}

/*******************************************************************
** Function: isNumber
** Description: check if a char is a number
** Parameters: a
** Pre-conditions: takes 1 char
** Post-conditions: check if a char is a number
*******************************************************************/

bool isNumber(char a){
	return (a>=48 && a<=57);
}

/*******************************************************************
** Function: checkRange
** Description: check if an int is between 2 limits
** Parameters: lower_bound, upper_bound, test_value
** Pre-conditions: 3 ints
** Post-conditions: check if test_value is between 2 limits
*******************************************************************/

bool checkRange(int lower_bound, int upper_bound, int test_value){
  if (lower_bound < 0) lower_bound = INT32_MIN;
  if (upper_bound < 0) upper_bound = INT32_MAX;
	return (lower_bound <= test_value && test_value <= upper_bound);
}

/*******************************************************************
** Function: equality_test
** Description: check if the 2 numbers are smaller, bigger or equal
** Parameters: num1, num2
** Pre-conditions: 2 ints
** Post-conditions: check if the 2 numbers are smaller, bigger or equal
*******************************************************************/

int equality_test(int num1, int num2){
	return clamp(num1 - num2, -1, 1);
}

/*******************************************************************
** Function: is_int
** Description: check if a string is an int
** Parameters: string
** Pre-conditions: 1 string
** Post-conditions: return true or false depending on if the string is a number or not
*******************************************************************/

bool is_int(std::string a){
	for(int i = 0; i < a.length(); i++){
		if(a[i] < 48 || a[i] > 57)
			if(!(i==0 && a[i] == '-'))
				return false;
	}
	return true;
}

/*******************************************************************
** Function: is_int
** Description: check if a string is an int
** Parameters: string
** Pre-conditions: 1 string
** Post-conditions: return true or false depending on if the string is a number or not
*******************************************************************/

bool is_int(char* a){
	for(int i = 0; i < strlen(a); i++){
		if(a[i] < 48 || a[i] > 57)
			if(!(i==0 && a[i] == '-'))
				return false;
	}
	return true;
}

/*******************************************************************
** Function: swap
** Description: swaps the value of 2 variables
** Parameters: num1, num2
** Pre-conditions: 2 int variables
** Post-conditions: none
*******************************************************************/

void swap(int &num1, int &num2){
	int temp = num1;
	num1 = num2;
	num2 = temp;
}

/*******************************************************************
** Function: word_count
** Description: counts the amount of words in a string
** Parameters: string
** Pre-conditions: 1 string
** Post-conditions: number of words
*******************************************************************/

int word_count(std::string sentence){
	int count = 1;
	std::string sentence1 = sentence + " ";
	for(int i = 1; i < sentence1.length(); i++){
		if((isLetter(sentence1[i-1])||isNumber(sentence1[i-1]))&&(sentence1[i]==' ' || sentence1[i]=='.' || sentence1[i]==',')){
			count++;
		}
	}
	return count;
}

/*******************************************************************
** Function: string_to_int
** Description: assign the number in a string to an int variable
** Parameters: string, int
** Pre-conditions: 1 string, 1 int variable
** Post-conditions: assign the number in a string to an int variable
*******************************************************************/

int string_to_int(std::string num){
	int counter = num.length()-1;
	int value = 0;
	for(int i = 0; i < num.length(); i++, counter--){
		if(num[counter]!='-')
			value += (num[counter] - 48)*pow(10,i);
	}
	if(num[0]=='-') value = -value;
	return value;
}

/*******************************************************************
** Function: string_to_int
** Description: assign the number in a string to an int variable
** Parameters: string, int
** Pre-conditions: 1 string, 1 int variable
** Post-conditions: assign the number in a string to an int variable
*******************************************************************/

int string_to_int(char* num){
	int counter = strlen(num)-1;
	int value = 0;
	for(int i = 0; i < strlen(num); i++, counter--){
		if(num[counter]!='-')
			value += (num[counter] - 48)*pow(10,i);
	}
	if(num[0]=='-') value = -value;
	return value;
}

/*********************************************************************
 ** Function: random
 ** Description: Generate a random number between 2 ints inclusive
 ** Parameters: lowerLimit(a) and upperLimit(b)
 ** Pre-Conditions: none
 ** Post-Conditions: none
 *********************************************************************/
int random(int a, int b) {
  if (b < a) return -1;
  else {
    return rand() % (b - a + 1) + a;
  }
}

/*********************************************************************
 ** Function: askForInt
 ** Description: Ask for a user input and will make sure the input will be an integer between 2 integers inclusive. Will also print custom prompt.
 ** Parameters: prompt, lowerLim, upperLim
 ** Pre-Conditions: limits must be larger than 0
 ** Post-Conditions: none
 *********************************************************************/

int askForInt(std::string prompt, int lowerLim, int upperLim) {
  //Constructing error message
  std::string errorMessage = "Please enter an integer ";

  if (lowerLim >= 0) {
    errorMessage += ">= " + std::to_string(lowerLim);
    if (upperLim > lowerLim) errorMessage += " and";
  }

  if (upperLim > lowerLim) errorMessage += " <= " + std::to_string(upperLim);

  //for range checking
  if (lowerLim < 0) lowerLim = INT32_MIN;
  if (upperLim < 0) upperLim = INT32_MAX;

  //print initial prompt
  std::cout << prompt;

  //read initial value
  int temp;
  std::cin >> temp;

  //determine initial range
  bool inRange = (temp >= lowerLim && temp <= upperLim);

  //while input is not valid or not in range
  while (!std::cin || !inRange) {
    //print error message and prompt
    std::cout << errorMessage << std::endl << prompt;
    //clear input cache cuz internet says so
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    //input
    std::cin >> temp;
    //check range
    if (temp >= lowerLim && temp <= upperLim) inRange = true;
  }

  return temp;
}

int checkArgumentNumber(char *argument, int lower_bound, int upper_bound, std::string error){
	if(is_int(argument)){
		int temp = string_to_int(argument);
		if(checkRange(lower_bound, upper_bound, temp)){
			return temp;
		}
	}
	std::cout << "Invalid arguments\n" << error << std::endl;
	exit(42);
}