#include <iostream>
#include <queue>
#include <climits>

#define INF 40000

/* PHASE 1 FUNCTIONS */ 
int max(int a, int b){
    if (a > b) {
        return a;
    } else {
        return b; 
    }
}

int strdex(std::string *strs, std::string input, int n)
{
    int index = INF;

    for(int i = 0; i < n; i++){
        if(strs[i] == input) index = i; 
    }

    return index; 
}


//DISTANCE ARRAY and partner FORBIDDEN ARRAY
int distarray[100][100];
void init_darray() {
    for (int c = 0; c < 100; c++){
        for (int r = 0; r < 100; r++) {
            distarray[c][r] = INF; 
        }
    }
}
//FORBID ARRAY
bool allowed[100][100]; 
void init_allowed()
{
    for (int i = 0; i < 100; i++ ){
        for (int j = 0; j < 100; j++) {
            allowed[i][j] = true;
            allowed[j][i] = true;
        }
    }
}


//recursive programming solution for least common subsequence. Returns length of lcs
int lcs(std::string s_a, std::string s_b, int a, int b) 
{
    //std::cout << "a = " << a << " b = " << b << std::endl;
    if (a == 0 || b == 0) return 0; 

    if (s_a[a - 1] == s_b[b - 1]) {
        return 1 + lcs(s_a, s_b, a -1 , b-1);
    } else {
        return max(lcs(s_a, s_b, a, (b-1)) , lcs(s_a, s_b, (a - 1), b) );
    }

}


//function for calculating distance between two strings
int cost(std::string a, std::string b)
{
    //std::cout <<  (a.size() - lcs(a, b, a.size(), b.size())) << std::endl;
    return (a.size() - lcs(a, b, a.size(), b.size())) << 1;
}

//function for filling in the array
void fillweights (std::string *strs, int n)
{
    for(int f = 0; f < n; f++){
        for(int t = 0; t < n; t++){
            if(f == t) {
                distarray[f][t] = 0;
            } else {
                distarray[f][t] = cost(strs[f], strs[t]) * cost(strs[f] , strs[t]);
            }
        }
    }

}

/* PHASE 2 FUNCTIONS */

//FLOYD WARSHALL APSP FUNCTION
//Will run once to initialize then when Forbid and Allow are called. 
void update_table (std::string *strs, int n) 
{
    fillweights(strs, n);
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n ; j++) {
            for (int i = 0; i < n ; i++) {
                if ( (distarray[i][j] > distarray[i][k] + distarray[k][j]) && allowed[i][j] && allowed[i][k] && allowed[k][j]){
                    distarray[i][j] = distarray[i][k] + distarray[k][j];
                } 
            }
        }
    }    

}


//diagnosis function
void print_darray(int a, int b)
{
    for (int i = 0 ; i < a; i++ ) {
        for (int j = 0; j < b; j++) {
            std::cout << distarray[i][j] << "/" << allowed[i][j] << " " ;
        } 
        std::cout << std::endl;
    }

    std::cout << "________" << std::endl;
}

int main ()
{
    /*TAKING AND PROCESSING INPUTS*/
    
    //initialize distance array
    init_darray();
    init_allowed();
    //number of strings
    int n; 
    std::cin >> n; 

    //number of commands
    int m; 
    std::cin >> m; 

    //string inputs
    std::string inputs[n];
    
    for (int i = 0; i < n; i++){
        std::cin >> inputs[i];
    }
   
    //take commands
    std::queue<std::string> commands;
    std::string command; 
    for (int i = 0; i < m; i++){
        std::cin >> command;

        //if Command = Cost, push 3 items to queue; The COST command itself, and the two Strings
        if (command == "COST") {
            commands.push(command);
            for (int j = 0; j < 2; j++){
                std::cin >> command; 
                commands.push(command);
            }

        //if command is either ALLOW or FORBID, push 2 items to the queue, the ALLOW/FORBID command and the string 
        } else if (command == "ALLOW" || command == "FORBID"){
            commands.push(command);
            std::cin >> command;
            commands.push(command);
        }
    }
   
    //initialize the table with weights
    fillweights(inputs, n);

    /* PHASE 2: COST, FORBID, ALLOW */
    
    //get all distances assuming all nodes are allowed
    update_table(inputs, n);

    int from, to, bnnd;
    std::queue<int> outputs; 
    //PROCESS COMMAND QUEUE
    while (!commands.empty()) {
        update_table(inputs, n);
        if (commands.front() == "COST" ) {
            commands.pop();
            from = strdex(inputs, commands.front(), n);
            commands.pop();
            to = strdex(inputs, commands.front(), n);
            commands.pop();

            outputs.push(distarray[from][to]);
            
        } else if (commands.front() == "FORBID") {
            commands.pop();
            bnnd = strdex(inputs, commands.front(), n);
            commands.pop();
            for (int i = 0; i < n; i++){
                allowed[bnnd][i] = false; 
                allowed[i][bnnd] = false;
            }
        } else if (commands.front() == "ALLOW") {
            commands.pop();
            bnnd = strdex(inputs, commands.front(), n);
            commands.pop();
            for (int i = 0; i < n; i++){
                allowed[bnnd][i] = true; 
                allowed[i][bnnd] = true;
            }
        }
    }
    
    //print outputs
    while(!outputs.empty()){
        std::cout << outputs.front() << std::endl;
        outputs.pop();
    }


    return 0;
}
