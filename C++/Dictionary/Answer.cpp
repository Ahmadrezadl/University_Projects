//Thanks to Mahdi Sabour
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <sstream> 
using namespace std;

int main(){
	string input;
	string data[50][10] = {""};
	ifstream file;
	file.open("db.txt");
	char temp;
	while(file.get(temp))
	{
		while(!isdigit(temp) && !file.eof())
			file.get(temp);
		string number;
		while(isdigit(temp) && !file.eof())
		{
			number += temp;
			file.get(temp);
		}
		stringstream geek(number); 
		int num;
		geek >> num;
		string newstring;
		while(temp != ',' && !file.eof() && temp != ']')
		{
			newstring += temp;
			file.get(temp);
		}
		int i =0;
		while(data[num][i] != "")i++;
		data[num][i] = newstring;
		if(file.eof())break;
	}
	file.close();
	while(1)
	{
		char s = getch();
		if ((int)s == 13)
		{
			string s = "start https://www.google.com/search?q=" + input;
			string tr="start https://translate.google.com/#en/de/"+input+"%0A";
			system(s.c_str());
			system(tr.c_str());
			continue;
		}
		if ((int)s == 8)
			input = input.substr(0,input.length()-1);
		else 
			input += s;
		
		system("cls");
		cout<<input<<endl<<"_________________"<<endl;
		int res = 0;
		if(input == "") continue;
		for(int i = 0; i < 50;i++)
		for(int k = 0;k< 10;k++)
		{

			bool found = true;
			if(input.length() != data[i][k].length())
				continue;
			for(int j = 0;j < data[i][k].length();j++)
			{
				if(tolower(data[i][k][j]) != tolower(input[j]))
					found = false;
			}
			if(found)
			{
				res++;
				for(int j = 0; j < 10;j++)
				{
					if(data[i][j] == "")
					continue;
					cout<<data[i][j]<<endl;
				}
				break;
			}
		}
		if(res==0)
		{
			cout<<"Not Found!"<<endl;
			for(int i = 0; i < 50;i++)
			for(int k =0;k < 10;k++)
			{
				if(data[i][k] == "")continue;
				if(input.length() > 1)
				{
					if(data[i][k].substr(1,data[i][k].length()).find(input.substr(1,input.length())) == 0
					&& tolower(data[i][k][0]) == tolower(input[0]))
					{
						cout<<data[i][k]<<endl;
						res++;
					}
				}
				else
				{
					if(tolower(data[i][k][0]) == tolower(input[0]))
					{
						cout<<data[i][k]<<endl;
						res++;
					}
				}

				
			}
		}
		if(res ==0)
		{
			int half = (input.length()/2)+1;
			for(int i = 0; i < 50;i++)
			for(int k = 0; k < 10;k++)
			{
				int same = 0;
				if(data[i][k].length() != input.length()) continue;
				for(int j = 0;j < input.length();j++)
				{
					if(tolower(data[i][k][j]) == tolower(input[j]))
					same++;
				}
				if(same >= half)
				{
					for(int j = 0;j< 10;j++)
					{
						if(data[i][j] == "") continue;
						cout<<data[i][j]<<endl;
					}
					cout<<"_________________"<<endl;
					break;
				}
			}
			
		}
	}
}
