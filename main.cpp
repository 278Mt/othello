#include<iostream>
#define BLACK 2
#define WHITE 1
#define SPACE 0

using namespace std;

bool all_space(int board[8][8]) {
    for(int i=0; i<8; i+=1) for(int j=0; j<8; j+=1) if(board[i][j]==SPACE) return false;
    return true;
}

void board_init(int board[8][8]) {
    for(int i=0; i<8; i+=1) for(int j=0; j<8; j+=1) board[i][j] = SPACE;

    board[3][3] = board[4][4] = WHITE;
    board[3][4] = board[4][3] = BLACK;
}

void dir_init(bool dir[3][3]) {
    for(int i=0; i<3; i+=1) for(int j=0; j<3; j+=1) dir[i][j] = false;
}

bool returnable(int board[8][8], int x, int y, int turn, bool dir[3][3]) {
    if(board[y][x] != SPACE) return false;

    dir_init(dir);

    for(int itrace=-1; itrace<=1; itrace+=1) {
        for(int jtrace=-1; jtrace<=1; jtrace+=1) {
            if(board[y+itrace][x+jtrace] == turn or board[y+itrace][x+jtrace] == SPACE or y+itrace < 0 or y+itrace >= 8 or x+jtrace < 0 or x+jtrace >= 8) continue;
            for(int i=itrace*2, j=jtrace*2; y+i<8 and x+j<8 and y+i>=0 and x+j>=0; i+=itrace, j+=jtrace)
                if(board[y+i][x+j] == turn) dir[itrace+1][jtrace+1] = true;
                else if(board[y+i][x+j] != turn and board[y+i][x+j] != SPACE) continue;
                else if(board[y+i][x+j] == SPACE) break;
        }
    }
    for(int i=0; i<3; i+=1) for(int j=0; j<3; j+=1) if(dir[i][j] == true) return true;
    return false;
}

bool pass(int board[8][8], int turn, bool dir[3][3]) {
    for(int i=0; i<8; i+=1) for(int j=0; j<8; j+=1) if(returnable(board, i, j, turn, dir)) return false;
    return true;
}

void compute_hint(int board[8][8], bool hint[8][8], int turn, bool dir[3][3]) {
    for(int i=0; i<8; i+=1) for(int j=0; j<8; j+=1)  hint[j][i] = returnable(board, i, j, turn, dir);
}

void count_bw(int board[8][8]) {
    int black = 0;
    int white = 0;
    for(int i=0; i<8; i+=1) for(int j=0; j<8; j+=1)
            if(board[i][j] == BLACK) black += 1;
            else if(board[i][j] == WHITE) white += 1;
    cout << "黒=" << black << " " << "白=" << white << " \x1b[41m　\x1b[49m=stonable"<< endl;
}

void display_board(int board[8][8], bool hint[8][8]) {
    cout << "------------------>x" << endl;
    cout << "| ０１２３４５６７" << endl;

    for(int i=0; i<8; i+=1) {
        cout << "|" << i;
        for(int j=0; j<8; j+=1)
            if(board[i][j] == BLACK) cout << "\x1b[42m\x1b[30m黒\x1b[39m\x1b[49m";
            else if(board[i][j] == WHITE) cout << "\x1b[42m\x1b[37m白\x1b[39m\x1b[49m";
            else if(board[i][j] == SPACE and hint[i][j]) cout << "\x1b[41m\x1b[33m"<< j << i <<"\x1b[39m\x1b[49m";
            else if(board[i][j] == SPACE and not hint[i][j]) cout << "\x1b[42m　\x1b[49m";
        cout << i << endl;
    }
    cout << "v ０１２３４５６７" << endl;
    cout << "y" << endl;

    count_bw(board);
}

void now_turn(int turn) {
    cout << "NOW: ";
    if(turn == BLACK) cout << "\x1b[42m\x1b[30m黒\x1b[39m\x1b[49m";
    else if(turn == WHITE) cout << "\x1b[42m\x1b[37m白\x1b[39m\x1b[49m";
    cout << endl;
}

bool input(int *x, int *y) {
    char strx[1024], stry[1024];
    cout << "(x, y): "; cin >> strx >> stry;
    if(strx[0] >= '0' and strx[0] < '8' and stry[0] >= '0' and stry[0] < '8' and strx[1] == 0 and stry[1] == 0) {
        *x = strx[0] - '0';
        *y = stry[0] - '0';
        return true;
    }
    return false;
}

void reverse(int board[8][8], int x, int y, int turn, bool dir[3][3]) {

    board[y][x] = turn;

    for(int itrace=-1; itrace<=1; itrace+=1) {
        for(int jtrace=-1; jtrace<=1; jtrace+=1) {
            if(dir[itrace+1][jtrace+1] == false) continue;
            for(int i=itrace, j=jtrace; board[y+i][x+j] != turn and board[y+i][x+j] != SPACE and y+i<8 and x+j<8 and y+i>=0 and x+j>=0; i+=itrace, j+=jtrace) board[y+i][x+j] = turn;
        }
    }
}

int change_turn(int turn) {
    if(turn == BLACK) return WHITE;
    else if(turn == WHITE) return BLACK;
    return -1;
}

void win(int board[8][8]) {
    int black = 0;
    int white = 0;
    for(int i=0; i<8; i+=1) for(int j=0; j<8; j+=1)
            if(board[i][j] == BLACK) black += 1;
            else if(board[i][j] == WHITE) white += 1;
    if(black == white) cout << "draw" << endl;
    else if(black > white) cout << "\"黒\" is winner!" << endl;
    else if(black < white) cout << "\"白\" is winner!" << endl;
}

int main(int argc, char **argv) {

    int x, y;
    int turn = BLACK, passtime = 0;

    int board[8][8];
    bool dir[3][3];
    bool hint[8][8];

    board_init(board);

    while(not all_space(board)) {

        if(pass(board, turn, dir)) {
            passtime+=1;
            turn = change_turn(turn);
            if(passtime == 2) break;
            else continue;
        }
        passtime = 0;

        compute_hint(board, hint, turn, dir);
        dir_init(dir);
        display_board(board, hint);
        now_turn(turn);
        while(not(input(&x, &y) and returnable(board, x, y, turn, dir))) {    // unable to return stone on the board
            cout << "unstonable." << endl;
            dir_init(dir);
            display_board(board, hint);
            now_turn(turn);
        }
        reverse(board, x, y, turn, dir);
        turn = change_turn(turn);
    }
    display_board(board, hint);
    win(board);

    return 0;
}
