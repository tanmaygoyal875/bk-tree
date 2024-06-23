// C++ program to demonstrate working of BK-Tree
#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx,avx2,fma")
#include <iostream>
//#include "bits/stdc++.h"
// #include <string>
// #include <string.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <math.h>
#include <cmath>
#include <unordered_map>
#include <math.h>
#include <set>
#include <unordered_set>
#include <iomanip>
#include <ios>
#include <queue>
using namespace std;


#define MAXN 1000000 // It defines the maximum words which can be present in the dictionary


int TOL=1; // The maximum tolerance value while suggesting a new word


#define LEN 100 //The maximum length of the word which can be handeled

struct Node
{
	string word;
	int next[2*LEN];
	Node(string x):word(x)
	{
		// initializing next[i] = 0
		for(int i=0; i<2*LEN; i++)
			next[i] = 0;
	}
	Node() {}
};
Node RT;
Node tree[MAXN];
int ptr; //index of the current node of the tree
int min(int a, int b, int c)
{
	return min(a, min(b, c));
}


int editDistance(string& a,string& b)
{
	int m = a.length(), n = b.length();
	int dp[m+1][n+1];

	for (int i=0; i<=m; i++) //base cases
		dp[i][0] = i;
	for (int j=0; j<=n; j++)
		dp[0][j] = j;

	for (int i=1; i<=m; i++)
	{
		for (int j=1; j<=n; j++)
		{
			if (a[i-1] != b[j-1])
			{
				dp[i][j] = min( 1 + dp[i-1][j], // deletion
								1 + dp[i][j-1], // insertion
								1 + dp[i-1][j-1] // replacement
							);
			}
			else
				dp[i][j] = dp[i-1][j-1];
		}
	}
	return dp[m][n];
}

void add(Node& root,Node& curr)
{
	if (root.word == "" )
	{
		root = curr; // The first node is defined as the root node.
		return;
	}

	int dist = editDistance(curr.word,root.word); //Finding the edit distance of the root word

	if (tree[root.next[dist]].word == "")
	{
		ptr++;
		tree[ptr] = curr;
		root.next[dist] = ptr;
	}
	else
	{
		add(tree[root.next[dist]],curr);
	}
}

vector <string> getSimilarWords(Node& root,string& s)
{
	vector < string > ret;
	if (root.word == "")
	return ret;

	// calculating editdistance of s from root
	int dist = editDistance(root.word,s);

	// if dist is less than tolerance value
	// add it to similar words
	if (dist <= TOL) ret.push_back(root.word);

	int start = dist - TOL;
	if (start < 0)
	start = 1;

	while (start <= dist + TOL)
	{
		vector <string> tmp =
			getSimilarWords(tree[root.next[start]],s);
		for (int i=0;i<tmp.size();i++)
			{
				if(ret.size()>=5) //returning after finding 5 closely related words
				return ret;
				ret.push_back(tmp[i]);
			}
		start++;
	}
	return ret;
}

void getwords(vector<string>&dictionary,string word)
{
	// dictionary words
	ptr = 0;
	int sz = dictionary.size();

	// adding dict[] words on to tree
	for(int i=0; i<sz; i++)
	{
		Node tmp = Node(dictionary[i]);
		add(RT,tmp);
	}
	vector < string > match = getSimilarWords(RT,word);

	match = getSimilarWords(RT,word);
	cout << "Correct words in dictionary for " << word << ":\n";
    if(match.size()==0)
    {
        cout<<"Sorry, no words for suggestion\n";
    }
	for (int i=0;i<match.size();i++)
		cout << match[i] << endl;
    cout<<"If you want to add this word in your dictionary press Y else press any other key to continue";
    char inp;
    cin>>inp;
    if(inp=='Y')
    dictionary.push_back(word);
    return;
}

int main()
{
	vector<string> dictionary;
   	ifstream input ("/usr/share/dict/words");
    for (std::string line; getline(input, line);) 
	dictionary.push_back(line);
    while(1)
    {
        cout<<"Enter 1:To insert word\n";
        cout<<"Enter 2:To view list of words present for suggestion\n";
		cout<<"Enter 3:To remove a particular word from dictionary\n";
        cout<<"Enter 4:To give suggestion of closely related word\n";
		cout<<"Enter 5:To change tolerance value\n";
        cout<<"Enter 6:To exit\n";
        int n;
        cin>>n;
        if(n==5)
        return 0;
        if(n==1)
        {
            string s;
            cin>>s;
            dictionary.push_back(s);
        }
        else if(n==2)
        {
            for(int i=0;i<dictionary.size();i++)
            {
                cout<<dictionary[i]<<"\n";
            }
        }
		else if(n==3)
		{
			cout<<"\n";
			cout<<"Enter the word to be removed: ";
			string s;
			cin>>s;
			dictionary.erase(find(dictionary.begin(),dictionary.end(),s));
			cout<<"The word has been removed\n";
		}
        else if(n==4)
        {
            string s;
            cin>>s;
            getwords(dictionary,s);
        }
		else if(n==5)
		{
			int n;
			cin>>n;
			TOL=n;
		}
        else
        return 0;
        cout<<"\n";
    }
}