#include <iostream>
#include <vector>
#include <string>

using namespace std;
int main()
{
	string word;
	vector<string> text;
	vector<string>::iterator i;
	//int i = 0;
	while (1)
	{
		cin>>word;
		if (word == "quit")
		{
			break;
		}
		text.push_back(word);
	}
	for (i = text.begin(); i != text.end(); i++)
	{
		cout<<*i<<"";
	}
	cout<<endl;
}
