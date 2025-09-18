//============================================================================
// Name         : Polynomial Calculator 
// Author       : Khalid Mengal
// Version      : 1.0
// Date Created : 25-08-2022
// Date Modified: 19-09-2022 
// Description  : Polynomial Calculator in C++ using Singly Linked-List
//============================================================================
#include<iostream>
#include<string>
#include<fstream>
#include<exception>
#include<sstream>
#include<iomanip>
#include "polycalculator.hpp"
using namespace std;

//=====================================
void listCommands()
{
	cout<<"List of available Commands:"<<endl
		<<"display              : Display the Polynomials"<<endl
		<<"input                : Input Polynomial expressions from keyboard"<<endl
		<<"add                  : Add the Polynomials (Exp1 + Exp2)"<<endl
		<<"sub                  : Subtract the Polynomials (Exp2 - Exp1)"<<endl
		<<"mul                  : Multiply the polynomials (Exp1 * Exp2)"<<endl
		<<"evaluate <ExpID,int> : Evaluate a polynomial for a specific value of x"<<endl
		<<"getDegree <ExpID>    : Returns the degree of a given polynomial."<<endl     
		<<"read <file_name>     : Load the data from <file> and add it into the Linked Lists"<<endl
		<<"help                 : Display the list of available commands"<<endl
		<<"exit                 : Exit the Program"<<endl;
}
//=======================================
// main function
int main()
{
	PolyCalculator polycalc;

	listCommands();
	string user_input;
	string command;
	string parameter1;
	string parameter2;
	do
	{
		try
		{
			cout<<">";
			getline(cin,user_input);
			
			// parse userinput into command and parameter(s)
			stringstream sstr(user_input);
			getline(sstr,command,' ');
			getline(sstr,parameter1,',');
			getline(sstr,parameter2);


			
			if(command =="display") 		polycalc.display();
			else if(command == "input")     polycalc.input();
			else if(command == "add")		polycalc.add();
			else if(command == "sub")		polycalc.sub();
			else if(command == "mul")		polycalc.mul();
			else if(command == "evaluate")	polycalc.evaluate(stoi(parameter1),stoi(parameter2));
			else if(command == "getDegree")    cout<<"The degree of Exp"<<parameter1<<" is: "<<polycalc.getDegree(stoi(parameter1))<<endl;
			else if(command =="read")		polycalc.read(parameter1),polycalc.display();
			else if(command == "help")		listCommands();
			else if(command == "exit" or command=="quit")			break;
			else 		cout<<"Invalid Command!!"<<endl;
		}
		catch(exception &ex)
		{
			cout<<"Exception: "<<ex.what()<<endl;
		}
	}while(true);

	return EXIT_SUCCESS;
}