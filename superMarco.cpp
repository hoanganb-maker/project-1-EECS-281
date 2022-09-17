// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include<iostream>
#include<string>
#include<unordered_set>
#include "Strategy.hpp"
using namespace std;

//REQUIRES: castle has the appropriate size to load the data
//MODIFIES: castle, cerr
//EFFECTS:  Load the characters to castle. If there is illegal map characters,
//          print error message to cerr and return the empty vector, 
//          else return the start position.
vector<int> load_castle(vector<vector<vector<char>>> &castle, int num_rooms, int size, char input_mode);



int main(int argc, char** argv){
    ios_base::sync_with_stdio(false);
    if(argc < 2){
        cerr << "Stack or queue must be specified";
        exit(1);
    }
    char strategy;
    if(argv[1] == string("--stack") || argv[1] == string("-s")){
        strategy = 'S';
    }
    else{
        strategy = 'Q';
    }
    int load = 2;
    char output_mode = 'M';
    while(true){
        if(load >= argc){
            break;
        }
        if(argv[load] == string("--stack") || argv[load] == string("-s") ||
           argv[load] == string("--queue") || argv[load] == string("-q")){
            cerr << "Stack or queue can only be specified once";
            exit(1);
        }
        load ++;
        output_mode = argv[load][0]; 
        break;
    }
    vector<vector<vector<char>>> castle;
    char input_mode;
    cin >> input_mode;
    int num_rooms;
    cin >> num_rooms;
    int size;
    cin >> size;
    castle.resize(num_rooms, vector<vector<char>>(size, vector<char>(size, '.')));

    vector<int> start;

    start = load_castle(castle, num_rooms, size, input_mode);
    if(start.empty()){
        exit(1);
    }
    Strategy myStrat(castle, start, strategy, output_mode);
    vector<int> end = myStrat.find_path();
    myStrat.print_path(end);
}

vector<int> load_castle(vector<vector<vector<char>>> &castle, int num_rooms, int size, char input_mode){
    string load;
    int room = 0;
    int row = 0;
    unordered_set<char> accepted_character = {'.', 'C', 'S', '#', '!', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    vector<int> start;

    //Load the new line character remains in the input stream
    getline(cin, load);
    if(input_mode == 'M'){
        while(getline(cin, load) && load.size()){
            if(load.substr(0, 2) == "//"){
                continue;
            }
            for(int col = 0; col < size; col++){
                if(accepted_character.find(load[col]) == accepted_character.end()){
                    cerr << "Unknown map character";
                    return {};
                }
                if(load[col] == 'S'){
                    start = {room, row, col};
                }
                castle[room][row][col] = load[col];
            }
            row++;
            if(row == size){
                row = 0;
                room++;
            }
            if(room == num_rooms){
                break;
            }
        }
    }
    else{
        while(getline(cin, load) && load.size()){
            if(load.substr(0, 2) == "//"){
                continue;
            }
            //room, row, col
            vector<int> castle_size(3, 0);
            int index = 1;
            for(int i = 0; i < 3; i++){
                while(isdigit(load[index])){
                    castle_size[i] = castle_size[i] * 10 + (load[index] - '0');
                    index++;
                }
                index++;
            }
            if(castle_size[0] >= num_rooms){
                cerr << "Invalid room number";
                return {};
            }
            if(castle_size[1] >= size){
                cerr << "Invalid row number";
                return {};
            }
            if(castle_size[2] >= size){
                cerr << "Invalid col number";
                return {};
            }
            if(accepted_character.find(load[index]) == accepted_character.end()){
                    cerr << "Unknown map character";
                    return {};
            }
            if(load[index] == 'S'){
                start = {castle_size[0], castle_size[1], castle_size[2]};
            }
            castle[castle_size[0]][castle_size[1]][castle_size[2]] = load[index];
        }
    }
    return start;
}


