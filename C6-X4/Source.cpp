
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

/*
This file is known as calculator02buggy.cpp

I have inserted 5 errors that should cause this not to compile
I have inserted 3 logic errors that should cause the program to give wrong results

First try to find an remove the bugs without looking in the book.
If that gets tedious, compare the code to that in the book (or posted source code)

Happy hunting!

*/

#include "../../std_lib_facilities.h"

//------------------------------------------------------------------------------

class Token {
public:
	char kind;        // what kind of token
	double value;     // for numbers: a value 
	Token(char ch)    // make a Token from a char
		:kind(ch), value(0) { }
	Token(char ch, double val)     // make a Token from a char and a double
		:kind(ch), value(val) { }
	void show();     // MA add to show contents of Token
};

//------------------------------------------------------------------------------

class Token_stream {
public:
	Token_stream();   // make a Token_stream that reads from cin
	Token get();      // get a Token (get() is defined elsewhere)
	void putback(Token t);    // put a Token back
private:
	bool full;        // is there a Token in the buffer?
	Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
	:full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
	if (full) error("putback() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

// The show() member function outputs the contents of the token:
void Token::show()
{
	cout << "Type = " << kind << " Value = " << value << "\n";
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
	if (full) {       // do we already have a Token ready?
					  // remove token from buffer
		full = false;
		return buffer;
	}

	char ch;
	cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

	switch (ch) {
	case '=':    // for "print"
	case 'x':    // for "exit"
	case '(': case ')': case '+': case '-': case '*': case '/': case '{': case '}': case '!':
		return Token(ch);        // let each character represent itself
								 // NEED to add factorial operator here
	case '.':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);         // put digit back into the input stream
		double val;
		cin >> val;              // read a floating-point number
		return Token('8', val);   // let '8' represent "a number"
	}
	default:
		error("Bad token");
	}
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

						//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

						//------------------------------------------------------------------------------

						// deal with numbers and parentheses
double factorial(int n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

double primary()
{
	cout << "---Entering primary()\n";
	Token t = ts.get();
	cout << "---"; t.show();
	switch (t.kind) {
	case '(':    // handle '(' expression ')'
	{
		double d = expression();
		t = ts.get();
		cout << "----Case ( "; t.show();
		if (t.kind != ')') error("')' expected");
		return d;
	}
	case '{':    // handle '{' expression '}'
	{
		double d = expression();
		t = ts.get();
		cout << "----Case { "; t.show();
		if (t.kind != '}') error("'}' expected");
		return d;
	}
	case '8':            // we use '8' to represent a number
		return t.value;  // return the number's value

	case 'x':
		ts.putback(t);
		return t.kind;
	default:
		error("primary expected");
	}
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
	cout << "--Entering term()\n";
	double left = primary();
	cout << "--Term back from primary()\n";

	Token t = ts.get();        // get the next token from token stream
	cout << "--"; t.show();
	if (t.kind == '!')
	{
		int temp = 0;
		temp = left;
		left = factorial(temp);
		t = ts.get();
	}

	while (true) {
		switch (t.kind) {
		case '*':
			left *= primary();
			t = ts.get();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			t = ts.get();
			break;
		}
		default:
			ts.putback(t);     // put t back into the token stream
			return left;
		}
	}
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
	cout << "-Entering expression()\n";
	double left = term();      // read and evaluate a Term
	cout << "-Expression - back from term()\n";
	Token t = ts.get();        // get the next token from token stream

	while (true) {
		switch (t.kind) {
		case '+':
			left += term();    // evaluate Term and add
			t = ts.get();
			break;
		case '-':
			left -= term();    // evaluate Term and subtract
			t = ts.get();
			break;
		default:
			ts.putback(t);     // put t back into the token stream
			return left;       // finally: no more + or -: return the answer
		}
	}
}

//------------------------------------------------------------------------------

int main()

try
{
	double value{ 0 };
	cout << "Welcome to the little calculator.\n";
	cout << "Please enter expressions using floating point numbers.\n";
	cout << "Supported operators: + - * / and parentheses.\n";
	cout << "'=' to display results.\n";
	cout << "'x' to exit.\n\n";
	while (cin) {
		Token t = ts.get();

		if (t.kind == 'x')    //Test for exit request and exit if indicated
		{
			cout << "Exiting application... \n";
			break; // 'x' for exit
		}
		if (t.kind == '=')        // '=' for "print now"
			cout << "=" << value << '\n';
		else
			ts.putback(t);
		value = expression();
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
