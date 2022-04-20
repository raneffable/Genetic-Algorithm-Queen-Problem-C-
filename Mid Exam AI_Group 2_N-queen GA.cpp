#include<iostream>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<sstream>
#include<cstring>
#include<map>
#include<list>
#include<queue>
#include<ctime>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

typedef struct
{
    string arrangement;
    int cost;
    int gen;
} solo;

typedef vector<solo*> population_type;

population_type population;
int chessBoardSize;
int initialPopulationCount;
int max_collision;

int fitnessValue(string arrangement)
{
    int fitness=max_collision;
    for(int i=0; i<chessBoardSize; i++)
        for(int j=i+1; j<chessBoardSize; j++)
            if((arrangement[i] == arrangement[j]) ||  (i-arrangement[i] == j-arrangement[j])
			|| (i+arrangement[i] == j+arrangement[j]))
                fitness--;
    return fitness;
}

bool isFit(solo *test)
{
    if(fitnessValue(test->arrangement)==max_collision)
        return true;
    return false;
}

solo* createNode()
{
    solo *newNode  = new solo;
    return newNode;
}

solo* generatePopulation()
{
	population.clear();
    string sampleArrangement="";
    solo *temp;
    for(int i=1; i<=chessBoardSize; i++)
    {
        ostringstream ostr;
        ostr<<i;
        sampleArrangement += ostr.str();
    }

    for(int i=0; i<initialPopulationCount; i++)
    {
        random_shuffle( sampleArrangement.begin(), sampleArrangement.end());
        temp   = createNode();
        temp->arrangement = sampleArrangement;
        temp->cost = fitnessValue(sampleArrangement);
        temp->gen = 1;
        if(isFit(temp))
            {
                return temp;
            }
        population.push_back(temp);
    }
    return temp;
}

solo* reproduce(solo *x, solo *y)
{
    solo *child = createNode();
    int n = chessBoardSize;
    int c = rand()%n;
    child->arrangement = (x->arrangement).substr(0,c) + (y->arrangement).substr(c,n-c);
    child->cost = fitnessValue(child->arrangement);
    child->gen = (x->gen)+1;
    return child;
}

solo* mutate(solo *child)
{
    int randomQueen = rand()%(chessBoardSize);
    int randomPosition= rand()%(chessBoardSize)+1;
    child->arrangement[randomQueen] = randomPosition+48;
    return child;
}

int randomSelection(int x)
{
	int randomPos = rand()%population.size();
	if(x == 2){
		randomPos = randomPos%2;
	}
    return randomPos;
}

bool comp(solo *a, solo*b)
{
    return(a->cost > b->cost);
}

solo* GA(int x)
{
	solo *child = generatePopulation();
    int randomNum1,randomNum2;
    solo *soloX,*soloY;
    bool found = 0;
    if(isFit(child)){
    	found = 1;
	}
    while(!found)
    {

        population_type new_population;
        if(x == 2){
        	sort(population.begin(),population.end(),comp);
		}
        for(unsigned int i=0; i<population.size(); i++)
        {

            randomNum1 = randomSelection(x);
            soloX = population[randomNum1];

            randomNum2 =randomSelection(x);
            soloY = population[randomNum2];

            child = reproduce(soloX,soloY);

            if(rand()%2==0)
                child = mutate(child);

            if(isFit(child))
            {
                found=1;
                return child;
            }
            new_population.push_back(child);
        }
        //cout<<"Generation:"<<child->gen<<endl;
        population = new_population;
    }
    return child;
}

int main()
{
	int max_solution[15] = {0,1,0,0,2,10,4,40,92,352,724,2680,14200,73712,365596};
	int menu;
	srand(time(NULL));
    cout << "Please input the chessboard size (at least 4 or 1)" << endl;
    cin >> chessBoardSize;
    while(chessBoardSize != 1 && chessBoardSize < 4){
        cout << "Invalid Input, please put proper number" << endl;
        cin >> chessBoardSize;
    }
    max_collision = (chessBoardSize*(chessBoardSize-1))/2;
    cout << "Please choose the algorithm that you want to use" << endl;
    cout << "1. Pure Random Genetic Algorithm (All gene can reproduce)" << endl;
    cout << "2. Chosen Genetic Algorithm (Only 2 highest fitness points gene can reproduce)" << endl;
    cin >> menu;
    while(menu != 1 && menu != 2){
        cout << "Invalid Input, please put proper number" << endl;
        cin >> menu;
    }
    cout << "Please input the population number (min 10)" << endl;
    cin >> initialPopulationCount;
    while(initialPopulationCount < 10){
        cout << "Invalid Input, please put proper number" << endl;
        cin >> initialPopulationCount;
    }
    map<string,int> solutionsFound;
    int choice = 0;
    int solution_found = 0;
    while(choice==0)
    {
        solo *solution = GA(menu);
        if(!solutionsFound[solution->arrangement])
        {

            solutionsFound[solution->arrangement]=1;
            solution_found += 1;
            cout << "Generation : " << solution->gen << endl;
            for(int i=0;i<chessBoardSize;i++){
                cout << "Queen number " << i+1 << " on column number " << (solution->arrangement[i])-48 << endl;
            }
            cout << endl;
            for(int i=0;i<chessBoardSize;i++){
                for(int j=0;j<chessBoardSize;j++){
                    cout << "|";
                    if((solution->arrangement[j])-48==i+1){
                        cout << "Q";
                    }
                    else{
                        cout <<" ";
                    }
                }
                cout << "|" << endl;
            }
        }
        else{
            continue;
        }
        if(chessBoardSize < 15 && (solution_found == max_solution[chessBoardSize])){
			cout << "There is no more solution to this problem" << endl;
			system("Pause");
			return 0;
		}
        cout << endl << "Do you want to find another solution? (0 = yes, 1 = no)" << endl;
	    cin >> choice;
	    while(choice < 0||choice > 1){
		    cout << "Invalid Input, please input 0/1" << endl;
		    cin >> choice;
	    }
    }
    system("Pause");
    return 0;
}
