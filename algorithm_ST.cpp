#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "algorithm.h"
#include "rules.h"

using namespace std;

#define ROW 5
#define COL 6

typedef struct Position {
    int x;
    int y;
    
    void Set(int i, int j) {
        x = i;
        y = j;
    }
} POS;

class BOARD{
    public:
        Cell cells[ROW][COL];                       
        void cell_reaction_marker();                
        bool cell_is_full(Cell* cell);              
        void add_orb(int i, int j, char color);     
        void cell_reset(int i, int j);              
        void cell_explode(int i, int j);            
        void cell_chain_reaction(Player player);   

    public:
        BOARD();
        BOARD(Board board);
        int get_orbs_num(int i, int j);
        int get_capacity(int i, int j);
        char get_cell_color(int i, int j);
        bool place_orb(int i, int j, Player* player);      
        bool win_the_game(Player player);                  
};

BOARD::BOARD(){
    cells[0][0].set_capacity(2), cells[0][5].set_capacity(2),
    cells[4][0].set_capacity(2), cells[4][5].set_capacity(2);
    cells[0][1].set_capacity(3), cells[0][2].set_capacity(3), cells[0][3].set_capacity(3), cells[0][4].set_capacity(3),
    cells[1][0].set_capacity(3), cells[2][0].set_capacity(3), cells[3][0].set_capacity(3),
    cells[1][5].set_capacity(3), cells[2][5].set_capacity(3), cells[3][5].set_capacity(3),
    cells[4][1].set_capacity(3), cells[4][2].set_capacity(3), cells[4][3].set_capacity(3), cells[4][4].set_capacity(3);
}

BOARD::BOARD(Board board) {
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            cells[i][j].set_color(board.get_cell_color(i, j));
            cells[i][j].set_capacity(board.get_capacity(i, j));
            cells[i][j].set_orbs_num(board.get_orbs_num(i, j));
        }
    }
}

bool BOARD::cell_is_full(Cell* cell){
    if(cell->get_orbs_num() >= cell->get_capacity()){
        cell->set_explode(true);
        return true;
    }
    else return false;
}

void BOARD::add_orb(int i, int j, char color){
    int orb_num = cells[i][j].get_orbs_num();
    orb_num ++;
    cells[i][j].set_orbs_num(orb_num);
    cells[i][j].set_color(color);
}

void BOARD::cell_reset(int i, int j){
    cells[i][j].set_orbs_num(0);
    cells[i][j].set_explode(false);
    cells[i][j].set_color('w');
}

void BOARD::cell_explode(int i, int j){

    int orb_num;
    char color = cells[i][j].get_color();

    cell_reset(i, j);

    if( i + 1 < ROW){
        add_orb(i+1, j, color);
    }

    if( j + 1 < COL){
        add_orb(i, j+1, color);
    }

    if( i - 1 >= 0){
        add_orb(i-1, j, color);
    }

    if( j - 1 >= 0){
        add_orb(i, j-1, color);
    }
}

bool BOARD::place_orb(int i, int j, Player * player){
    
    if(!index_range_illegal(i, j) && !placement_illegal(*player, cells[i][j])){
        int temp = cells[i][j].get_orbs_num();
        temp += 1;
        cells[i][j].set_orbs_num(temp);
        cells[i][j].set_color(player->get_color());
    }
    else{
        player->set_illegal();
        return false;
    }

    if(cell_is_full(&cells[i][j])){
        cell_explode(i, j);
        cell_reaction_marker();
        cell_chain_reaction(*player);
    }

    return true;
}

void BOARD::cell_reaction_marker(){
    for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                cell_is_full(&cells[i][j]);
            }
        }
}

void BOARD::cell_chain_reaction(Player player){
    
    bool chain_reac = true;

    while(chain_reac){
        chain_reac = false;
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                if(cells[i][j].get_explode()){
                    cell_explode(i ,j);
                    chain_reac = true;
                }
            }
        }
        if(win_the_game(player)){
            return;
        }
        cell_reaction_marker();
    }
}

bool BOARD::win_the_game(Player player){

    char player_color = player.get_color();
    bool win = true;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(cells[i][j].get_color() == player_color || cells[i][j].get_color() == 'w') continue;
            else{
                win = false;
                break;
            }
        }
        if(!win) break;
    }
    
    return win;
}

int BOARD::get_orbs_num(int i, int j){
    return cells[i][j].get_orbs_num();
}

int BOARD::get_capacity(int i, int j){
    return cells[i][j].get_capacity();
}

char BOARD::get_cell_color(int i, int j){
    return cells[i][j].get_color();
}

POS *SearchNeigbours(int i, int j);
int *SearchChain(BOARD board, Player player);
BOARD move(BOARD board, POS pos, Player player);
int score(BOARD board, Player player);
std::pair<POS, int> minimax(BOARD board, int depth, int breadth, Player player);

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions
 * 1. ~ 4. are listed in next block)
 *
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 *
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/


void algorithm_A(Board board, Player player, int index[]){
    BOARD first_board(board);
    pair <POS, int> best_move = minimax(first_board, 2, 10, player);
    int row, col, color;
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }

    index[0] = best_move.first.x;
    index[1] = best_move.first.y;
};

int *best_place(BOARD board, Player player) {
    int *conf = new int[100];
    for(int i = 0; i < 100; i++) {
        conf[i] = -10000;
    }
    char playerColor = player.get_color();
    
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(board.get_cell_color(i, j) == playerColor || board.get_cell_color(i, j) == 'w') {
                POS pos;
                pos.Set(i, j);
                conf[i*10+j] = score(move(board, pos, player), player);
            }
        }
    }
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            int k = 0;
            if(board.get_cell_color(i, j) == 'w'){
                k++;
            }
        }
    }
    return conf;
}

std::pair<POS, int> minimax(BOARD board, int depth, int breadth, Player player) {
    char playerColor = player.get_color();
    char opponentColor = (playerColor == 'r') ? 'b' : 'r';
    int *best_moves = best_place(board, player);
    POS best_pos[breadth];
    int best_val[breadth];
    POS best_next_pos;
    int best_next_val;
    
    for(int i = 0; i < breadth; i++) {
        best_pos[i].Set(0, 0);
        best_val[i] = -10000;
    }
    
    for(int i = 0; i < 100; i++) {
        if(i/10 > 4 || i%10 > 5) continue;
        if(board.get_cell_color(i/10, i%10)==opponentColor) continue;
        for(int j = 0; j < breadth; j++) {
            if(best_moves[i] > best_val[j]) {
                for(int k = breadth-1; k >= j+1; k--) {
                    best_val[k] = best_val[k-1];
                    best_pos[k].Set(best_pos[k-1].x, best_pos[k-1].y);
                }
                best_val[j] = best_moves[i];
                best_pos[j].Set(i/10, i%10);
                break;
            }
        }
    }
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            int k = 0;
            if(board.get_cell_color(i, j) == 'w'){
                k++;
            }
        }
    }
    delete []best_moves;
    best_next_pos = best_pos[0];
    best_next_val = score(move(board, best_next_pos, player), player);
    
    if(depth==1) {
        return std::make_pair(best_next_pos, best_next_val);
    }
    
    for(int i = 0; i < breadth; i++) {
        BOARD b_new(move(board, best_pos[i], player));
        pair <POS, int> best_move = minimax(b_new, depth-1, breadth, player);
        int val = best_move.second;
        if(val > best_next_val) {
            best_next_val = val;
            best_next_pos = best_pos[i];
        }
    }
    
    return std::make_pair(best_next_pos, best_next_val);
}

BOARD move(BOARD board, POS pos, Player player) {
    char playerColor = player.get_color();
    board.place_orb(pos.x, pos.y, &player);
    return board;
}

int score(BOARD board, Player player) {
    int sc = 0;
    int my_orbs = 0, enemy_orbs = 0;
    char playerColor = player.get_color();
    char opponentColor = (playerColor == 'r') ? 'b' : 'r';
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL ; j++) {
            if(board.get_cell_color(i, j) == playerColor) {
                my_orbs += board.get_orbs_num(i, j);
                bool flag_not_vulnerable = true;
                POS *Neighbours = SearchNeigbours(i, j);
                for(int k = 0; k < 4; k++) {
                    if(Neighbours[k].x == -1 && Neighbours[k].y == -1) {
                        break;
                    }
                    else {
                        if(board.get_cell_color(Neighbours[k].x, Neighbours[k].y) == opponentColor && board.get_orbs_num(Neighbours[k].x, Neighbours[k].y) == board.get_capacity(Neighbours[k].x, Neighbours[k].y)-1) {
                            sc -= (5 - board.get_capacity(Neighbours[k].x, Neighbours[k].y));
                            flag_not_vulnerable = false;
                        }
                    }
                }
                if(flag_not_vulnerable) {
                    if(board.get_capacity(i, j) == 3) {
                        sc += 2;
                    }
                    else if(board.get_capacity(i, j) == 2) {
                        sc += 3;
                    }
                    if(board.get_orbs_num(i, j) == board.get_capacity(i, j) - 1) {
                        sc += 2;
                    }
                }
                delete []Neighbours;
            }
            else {
                enemy_orbs += board.get_orbs_num(i, j);
            }
        }
    }
    
    sc -= enemy_orbs;
    
    if(enemy_orbs==0 && my_orbs>1) {
        return 10000;
    }
    else if(my_orbs==0 && enemy_orbs>1) {
        return -10000;
    }
    
    return sc;
}

POS *SearchNeigbours(int i, int j) {
    POS *Neighbours = new POS[8];
    for(int k = 0; k < 8; k++) {
        Neighbours[k].Set(-1, -1);
    }
    int num = 0;
    if(i==0) {
        Neighbours[num++].Set(i+1, j);
    }
    else if(i==ROW-1) {
        Neighbours[num++].Set(i-1, j);
    }
    else {
        Neighbours[num++].Set(i-1, j);
        Neighbours[num++].Set(i+1, j);
    }
    if(j==0) {
        Neighbours[num++].Set(i, j+1);
    }
    else if(j==COL-1) {
        Neighbours[num++].Set(i, j-1);
    }
    else {
        Neighbours[num++].Set(i, j-1);
        Neighbours[num++].Set(i, j+1);
    }
    
    return Neighbours;
}

int *SearchChain(BOARD board, Player player) {
    int *lengths = new int[31];
    int current = 0;
    char playerColor = player.get_color();
    char opponentColor = (playerColor == 'r') ? 'b' : 'r';
    
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(board.get_orbs_num(i, j) == (board.get_capacity(i, j)-1) && board.get_cell_color(i, j) == playerColor) {
                int l = 0;
                int num = 0;
                POS *visiting_stack = new POS[31];
                visiting_stack[num].Set(i, j);
                while(num>=0) {
                    POS pos = visiting_stack[num--];
                    board.cell_reset(pos.x, pos.y);
                    l += 1;
                    POS *Neighbours = SearchNeigbours(pos.x, pos.y);
                    for(int k = 0; k < 4; k++) {
                        if(Neighbours[k].x == -1 && Neighbours[k].y == -1) {
                            break;
                        }
                        else {
                            int x = Neighbours[k].x;
                            int y = Neighbours[k].y;
                            if(board.get_orbs_num(x, y) == (board.get_capacity(x, y)-1)) {
                                if(board.get_cell_color(x, y) == playerColor) {
                                    visiting_stack[++num].Set(x, y);
                                }
                                else if(board.get_cell_color(x, y) == opponentColor) {
                                    if(l>0) {
                                        l *= -1;
                                    }
                                }
                            }
                        }
                    }
                }
                lengths[++current] = l;
                delete []visiting_stack;
            }
        }
    }
    lengths[0] = current;
    
    return lengths;
}
