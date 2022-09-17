// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include<deque>
#include<unordered_map>
#include<vector>
using namespace std;

class Strategy{
    private:
    //A hash function to get the index of the vector below.
    //This works as if we treated the castle as a 1D vector and 
    //return the position for the 1D vector for each position in the
    //actual 3D vector.
    struct hasher{
        int size;
        int operator()(vector<int> pos) {
            return pos[0] * size * size + pos[1] * size + pos[2];
        }
    };
    hasher cor;

    // a vector of viewed positions.
    // each position in the vector is one of '$', 'n', 'e', 's', 'w', or a char number
    // (for warp pipe) indicating the move of the previous position. 
    vector<char> positions;

    // a queue of a stack of positions that could be viewed.
    deque<vector<int>> path;
    
    vector<vector<vector<char>>> castle;
    int num_rooms;
    int size;
    //keep track of num visited position in case C is not found. 
    int num_visited;

    vector<int> start;
    bool is_q_scheme;
    bool is_M_mode;

    public:
    
    //Constructor 
    Strategy(vector<vector<vector<char>>> &castle_in, vector<int> start_in, char strategy, char output_mode){
                castle = castle_in;
                start = start_in;
                is_q_scheme = (strategy == 'Q');
                is_M_mode = (output_mode == 'M');
                num_rooms = (int)castle_in.size();
                size = ((int)castle_in[0].size());
                cor.size = size;
                positions.resize(num_rooms * size * size, '$');
                positions[cor(start)] = 'S';
                path.push_back(start);
                num_visited = 1;
    }

    
    //MODIFIES: positions, path 
    //EFFECTS:  return the position (room, row, col) of the Countess if we can get there
    //          else return the empty vector. 
    vector<int> find_path(){
                    //Will use later to add next walkable spaces
                    vector<vector<int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
                    vector<char> directions = {'n', 'e', 's', 'w'};

                    while(!path.empty()){
                        vector<int> look;
                        if(is_q_scheme){
                            look = path.front();
                            path.pop_front();
                        }
                        else{
                            look = path.back();
                            path.pop_back();
                        }
                        int room = look[0];
                        int row = look[1];
                        int col = look[2];
                        if(isdigit(castle[room][row][col])){
                            //warp pipe
                            int teleport_room = castle[room][row][col] - '0';
                            if(teleport_room >= num_rooms || castle[teleport_room][row][col] == '#' ||
                            castle[teleport_room][row][col] == '!' || 
                            positions[cor({teleport_room, row, col})] != '$'){
                                continue;
                            }
                            //positions is set to the number of the previous room
                            else if(castle[teleport_room][row][col] == 'C'){
                                positions[cor({teleport_room, row, col})] = (char)((int)'0' + room);
                                return {teleport_room, row, col};
                            }
                            else{
                                path.push_back({teleport_room, row, col});
                                num_visited++;
                                positions[cor({teleport_room, row, col})] = (char)((int)'0' + room);
                            }
                        }
                        else{
                            for(int i = 0; i < 4; i++){
                                int dr = moves[i][0];
                                int dc = moves[i][1];
                                if(!forbide(room, row + dr, col + dc)){
                                    positions[cor({room, row + dr, col + dc})] = directions[i];
                                    if(castle[room][row + dr][col + dc] == 'C'){
                                        return {room, row + dr, col + dc};
                                    }
                                    path.push_back({room, row + dr, col + dc});
                                    num_visited++;
                                }
                            }
                        }
                    }
                    return {};
    }

    //end is the coordinate of the Countess, empty if we couln't find her. 
    //MODIFIES: cout, castle 
    void print_path(vector<int> end){
        if (end.empty()){
            cout << "No solution, " << num_visited << " tiles discovered.";
            return;
        }
        deque<char> directions;
        deque<vector<int>> travels;

        //Will be used to get back the path to start
        unordered_map<char, vector<int>> reverse_moves = {{'n', {1, 0}}, {'e', {0, -1}}, {'s', {-1, 0}}, {'w', {0, 1}}};
        vector<int> travel = end;

        // initially set positions[cor(start)] = 'S', so stop there.
        while(positions[cor(travel)] != 'S'){
            char direction = positions[cor(travel)];
            if(isdigit(direction)){
                castle[(direction - '0')][travel[1]][travel[2]] = 'p';
                travel = {(direction - '0'), travel[1], travel[2]};
                travels.push_front(travel);
                directions.push_front('p');
            }
            else{
                int r_prev = travel[1] + reverse_moves[direction][0];
                int c_prev = travel[2] + reverse_moves[direction][1];
                castle[travel[0]][r_prev][c_prev] = direction;
                travel = {travel[0], r_prev, c_prev};
                travels.push_front(travel);
                directions.push_front(direction);
            }
        }

        if(is_M_mode){
            cout << "Start in room " << start[0] << ", row " << start[1] << ", column " << start[2] << "\n";
            for(int room = 0; room < num_rooms; room++){
                cout << "//castle room " << room << "\n";
                for(int row = 0; row < size; row++){
                    for(int col = 0; col < size; col++){
                        cout << castle[room][row][col]; 
                    }
                    cout << "\n";
                }
            }
        }
        else{
            cout << "Path taken:" << "\n";
            auto dir_iter = directions.begin();
            auto tra_iter = travels.begin();
            while(dir_iter != directions.end()){
                vector<int> pos = *tra_iter;
                cout << "(" << pos[0] << "," << pos[1] 
                     << "," << pos[2] << "," << *dir_iter << ")" << "\n";
                tra_iter++;
                dir_iter++;
            }
        }
    }

    //Return true if the next position is out-of-bound, a minion, or a wall, or previously viewed.
    bool forbide(int room, int row, int col){
        return (row < 0 || row >= size || col < 0 || col >= size || 
                castle[room][row][col] == '!' || castle[room][row][col] == '#' || 
                positions[cor({room, row, col})] != '$');
    }
};



