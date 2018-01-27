
/*
Exercise 4 Chapter 6 from
"Software - Principles and Practice using C++" by Bjarne Stroustrup
EXERCISE: Define a class Name_value that holds a string and a value. 
Rework exercise 19 in Chapter 4 to use a vector<Name_value> instead of two vectors.

EX 19 From Chapter 4:
Write a program where you first enter a set of name-and-value pairs, 
such as Joe 17 and Barbara 22. For each pair, add the name to a vector 
called names and the number to a vector called scores 
(in corresponding positions, so that if names[7]=="Joe" then scores[7]==17). 
Terminate input with NoName 0. 
Check that each name is unique and terminate with an error message if a name is entered twice. 
Write out all the (name,score) pairs, one per line.
*/

#include "../../std_lib_facilities.h"

//------------------------------------------------------------------------------
class NameScore {
public:
	string Name;		// to hold name
	int	Score;			// to hold score
	NameScore(string s)						// make a NameScore from a name
		:Name(s), Score(0) { }
	NameScore(int val)						// make a NameScore from a score
		:Name("Nobody"), Score(val) { }
	NameScore(string s, int val)			// make a NameSCore from a score and name
		:Name(s), Score(val) { }
	void show();							// output the name and score
};

//------------------------------------------------------------------------------

int main()

try
{
	vector<NameScore> Names;
	NameScore tempNameScore{ "Nobody", 0 };
	
	bool keep_going = true;
	string temp_s{ "poop" };
	int temp_i{ 100 };

	cout << "Welcome to the Name-Score program ver. 2.0\n";
	cout << "Please enter Name Score pairs,\n";
	cout << "Duplicate names are not permitted.\n";
	cout << "NoName 0 to terminate.\n";

	while (keep_going) {
		cin >> temp_s;
		cin >> temp_i;


		// Check for duplicates //

		for (int i = 0; i < Names.size(); i++) {
			if (temp_s == Names[i].Name) {
				keep_going = false;
				error("Duplicate Names not allowed!\n");
			}
		}
		if (temp_s == "NoName" && temp_i == 0) {
			keep_going = false;
			break;
		}
		tempNameScore = NameScore(temp_s, temp_i);
		Names.push_back(tempNameScore);
	}
	for (int j = 0; j < Names.size(); j++) {
		Names[j].show();
	}
	keep_window_open();
}
catch (exception& e) {
	cerr << "error: " << e.what() << '\n';
	keep_window_open();
	return 1;
}
catch (...) {
	cerr << "Oops: unknown exception!\n";
	keep_window_open();
	return 2;
}

//------------------------------------------------------------------------------

void NameScore::show()
{
	cout << Name << "\t\t" << Score << "\n";
}
