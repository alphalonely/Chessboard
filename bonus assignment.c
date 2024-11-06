#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 8

char* rook_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user);
char* bishop_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user);
char* queen_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user);
char* pawn_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user);
char* knight_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user);
char* king_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user);


void print_board(char board[SIZE][SIZE]){
    printf("   ");
    for(int i = 0; i < SIZE; i++)
       printf("%d ", i);
    printf("\n");

    for(int i = 0; i < SIZE; i++){
       printf("%d  ", i);
       for(int j = 0; j < SIZE; j++)
         printf("%c ", board[i][j]);
       printf("\n");
    }
    return;
}

int get_position(int *r, int *c){
        int success = scanf("%d %d", r, c);
        if(success == 2) return 1;
        success = scanf("%d,%d", r, c);
        if(success == 2) return 1;
        success = scanf("%d-%d", r, c);
        if(success == 2) return 1;
        success = scanf("(%d,%d)", r, c);
        if(success == 2) return 1;
        int ch; //Clear the input      
        while ((ch = getchar()) != '\n' && ch != EOF);
        return 0;
}

bool check_king(char board[SIZE][SIZE], int king_r, int king_c, bool user) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            char piece = board[r][c];
            if ((user && piece >= 'A' && piece <= 'Z') || (!user && piece >= 'a' && piece <= 'z')) {
                if (piece == 'r' || piece == 'R') {
                    if (rook_move(board, r, c, king_r, king_c, !user) != NULL)
                        return true;
                } else if (piece == 'b' || piece == 'B') {
                    if (bishop_move(board, r, c, king_r, king_c, !user) != NULL)
                        return true;
                } else if (piece == 'q' || piece == 'Q') {
                    if (queen_move(board, r, c, king_r, king_c, !user) != NULL)
                        return true;
                } else if (piece == 'n' || piece == 'N') {
                    if (knight_move(board, r, c, king_r, king_c, !user) != NULL)
                        return true;
                } else if (piece == 'p' || piece == 'P') {
                    if (pawn_move(board, r, c, king_r, king_c, !user) != NULL)
                        return true;
                } else if (piece == 'k' || piece == 'K') {
                    if (king_move(board, r, c, king_r, king_c, !user) != NULL)
                        return true;
                }
            }
        }
    }
    return false;
}

bool checkmate(char board[SIZE][SIZE], bool user) {
    int king_r = -1, king_c = -1;

    // Find the king's position
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if ((user && board[r][c] == 'k') || (!user && board[r][c] == 'K')) {
                king_r = r;
                king_c = c;
                break;
            }
        }
        if (king_r != -1) break;
    }

    if (king_r == -1) return false; // No king found, edge case

    // Check if the king is in check
    if (check_king(board, king_r, king_c, user)) {
        return false; // Not in check, so not checkmate
    }

    // Try all possible moves of the king to see if one escapes check
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue; // Skip the king staying in place

            int new_r = king_r + dr;
            int new_c = king_c + dc;

            // Check if move is within bounds
            if (new_r >= 0 && new_r < SIZE && new_c >= 0 && new_c < SIZE) {
                // Temporarily make the move
                char temp = board[new_r][new_c];
                board[new_r][new_c] = board[king_r][king_c];
                board[king_r][king_c] = '.';

                // Check if the king is still in check
                bool in_check = check_king(board, new_r, new_c, user);

                // Undo the move
                board[king_r][king_c] = board[new_r][new_c];
                board[new_r][new_c] = temp;

                if (!in_check) {
                    return false; // Found a move that gets the king out of check
                }
            }
        }
    }

    // If no valid move was found, it is checkmate
    return true;
}

void find_king_position(char board[SIZE][SIZE], bool user, int *king_r, int *king_c) {
    char king = user ? 'k' : 'K';
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] == king) {
                *king_r = r;
                *king_c = c;
                return;
            }
        }
    }
    *king_r = -1; // If no king found, set to an invalid position
    *king_c = -1;
}

int main() {
    char board[SIZE][SIZE] = { 
        "RNBQKBNR",
        "PPPPPPPP",
        "........",
        "........",
        "........",
        "........",
        "pppppppp",
        "rnbqkbnr"
    };

    print_board(board);

    bool user = true;
    char *moved_piece;
    int r, c, new_r, new_c;
    int king_r, king_c;

    //We skip any kind of check for game end or winning conditions, so we have an endless loop
    while(1){
        find_king_position(board, user, &king_r, &king_c);
        
        if (check_king(board, king_r, king_c, user)) {
            if (checkmate(board, user)) {
                printf("User %d is in checkmate. Game over.\n", user ? 1 : 2);
                break;
            }
            printf("User %d is in check!\n", user ? 1 : 2);
        }
        printf("User %d: Please choose a piece (provide r and c):\n", user ? 1 : 2); 
        while(1){
            if(get_position(&r, &c))
               break;
            printf("Error. Could not read input, try again.\n");
        } 
        printf("User %d: Please provide destination (provide new_r and new_c):\n", user ? 1 : 2); 
        while(1){
            if(get_position(&new_r, &new_c))   
               break;
            printf("Error. Could not read input, try again.\n");
        }

        int success = 0;

        //try all moves until one succeeds
        moved_piece = rook_move(board, r, c, new_r, new_c, user);
        if(moved_piece) {
           printf("Rook moved successfully!\n");
           success = 1;
        }
        moved_piece = bishop_move(board, r, c, new_r, new_c, user);
        if(moved_piece) {
           printf("Bishop moved successfully!\n");
           success = 1;
        }
        moved_piece = queen_move(board, r, c, new_r, new_c, user);
        if(moved_piece) {
           printf("Queen moved successfully!\n");
           success = 1;
        }
        moved_piece = pawn_move(board, r, c, new_r, new_c, user);
        if(moved_piece) {
           printf("Pawn moved successfully!\n");
           success = 1;
        }
        moved_piece = knight_move(board, r, c, new_r, new_c, user);
        if(moved_piece) {
           printf("Knight moved successfully!\n");
           success = 1;
        }
        moved_piece = king_move(board, r, c, new_r, new_c, user);
        if(moved_piece) {
           printf("King moved successfully!\n");
           success = 1;
        }

        if(success) {
          print_board(board);
          user = !user; //change turn
        } else{
           printf("Error. Could not make move, invalid position or piece. Please try again.\n\n");
        }
    }

    return 0;
}

char* rook_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user) {
    //Validate the piece is a rook
    char piece = board[r][c];
    if ((user && piece != 'r') || (!user && piece != 'R'))
        return NULL;

    //Validate the move is in a straight line
    if (r != new_r && c != new_c)
        return NULL;

    //Validate we moved
    if(r == new_r && c == new_c)
        return NULL;


    //check all positions aside from start/end
    //Validate no pieces are in the path
    if(r == new_r && c > new_c){
        for(int i = new_c+1; i < c; i++)
          if(board[r][i] != '.')
             return NULL;
    } else if(r == new_r && c < new_c){
        for(int i = c+1; i < new_c; i++)
          if(board[r][i] != '.')
             return NULL;
    }

    if(c == new_c && r > new_r){
        for(int i = new_r+1; i < r; i++)
          if(board[i][c] != '.')
            return NULL;
    } else if(c == new_c && r < new_r){
        for(int i = r+1; i < new_r; i++)
          if(board[i][c] != '.')
            return NULL;
    }

    //Check the destination
    char dest_piece = board[new_r][new_c];
    if(user && (dest_piece >= 'a' && dest_piece <= 'z'))
       return NULL;
    if(!user && (dest_piece >= 'A' && dest_piece <= 'Z'))
       return NULL;

    //Move the rook and capture if necessary
    board[new_r][new_c] = piece;
    board[r][c] = '.';

    return &board[new_r][new_c];
}

char* bishop_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user) {
    //Validate the piece is a bishop
    char piece = board[r][c];
    if ((user && piece != 'b') || (!user && piece != 'B'))
        return NULL;

    //Validate the move is diagonal
    if (abs(new_r - r) != abs(new_c - c))
        return NULL;

    //Validate we moved
    if(r == new_r && c == new_c)
        return NULL;

    //Check if a piece is in the way diagonally (4 directions)
    if (new_r > r && new_c > c) { // moving down-right
        for (int i = r + 1, j = c + 1; i < new_r && j < new_c; i++, j++) 
            if (board[i][j] != '.') 
                return NULL;
    } else if (new_r > r && new_c < c) { // moving down-left
        for (int i = r + 1, j = c - 1; i < new_r && j > new_c; i++, j--) 
            if (board[i][j] != '.') 
                return NULL;
    } else if (new_r < r && new_c > c) { // moving up-right
        for (int i = r - 1, j = c + 1; i > new_r && j < new_c; i--, j++) {
            if (board[i][j] != '.') 
                return NULL;
            printf("%c\n", board[i][j]);
            }
    } else if (new_r < r && new_c < c) {// moving up-left
        for (int i = r - 1, j = c - 1; i > new_r && j > new_c; i--, j--) 
            if (board[i][j] != '.') 
                return NULL;
    }

    //Check the destination
    char dest_piece = board[new_r][new_c];
    if(user && (dest_piece >= 'a' && dest_piece <= 'z'))
       return NULL;
    if(!user && (dest_piece >= 'A' && dest_piece <= 'Z'))
       return NULL;

    //Move the rook and capture if necessary
    board[new_r][new_c] = piece;
    board[r][c] = '.';

    return &board[new_r][new_c];
}

char* queen_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user) {
    char piece = board[r][c];
    char queen_piece = user ? 'q' : 'Q';
    char rook_piece = user ? 'r' : 'R';
    char bishop_piece = user ? 'b' : 'B';

    if (piece != queen_piece)
        return NULL;

    //Check rook move for queen
    board[r][c] = rook_piece;
    char* moved_piece = rook_move(board, r, c, new_r, new_c, user);
    if (moved_piece != NULL) { //Success
        *moved_piece = queen_piece;
        return moved_piece;
    }

    //Check bishop move for queen
    board[r][c] = bishop_piece;
    moved_piece = bishop_move(board, r, c, new_r, new_c, user);
    if (moved_piece != NULL) { //Success
        *moved_piece = queen_piece;
        return moved_piece;
    }

    //Revert changes and return NULL on failure
    board[r][c] = queen_piece;
    return NULL;
}

char* pawn_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user) {
    //check if it is a pawn
    char piece = board[r][c];
    if ((user && piece != 'p') || (!user && piece != 'P')) {
        return NULL;
    }
    //check if pawn moved
    if (r == new_r && c == new_c) {
        return NULL;
    }

    int direction = user ? -1 : 1;
    int initial = user ? 6 : 1;

    //initial move(2 steps)
    if (r == initial && new_r == r + direction*2 && c == new_c) {
        if (board[new_r][new_c] != '.' || board[r + direction][c] != '.') {
            return NULL;
        }
    }
    //regular move (1 step)
    else if (new_r == r + direction && c == new_c) {
        if (board[new_r][new_c] != '.') {
            return NULL;
        }
    }

    //capturing
    else if (new_r == r + direction && (new_c == c + 1 || new_c == c - 1)) {
        if (board[new_r][new_c] == '.' || (user && (board[new_r][new_c] >= 'a' && board[new_r][new_c] <= 'z')) ||
            (!user && (board[new_r][new_c] >= 'A' && board[new_r][new_c] <= 'Z'))) {
            return NULL;
        }
    } else {
        return NULL;
    }

    //promotion
    if ((user && new_r == 0) || (!user && new_r == 7)) {
        char choice;
        printf("Choose a piece to promote (q/r/b/n): ");
        while (1) {
            scanf(" %c", &choice);
            if (choice == 'q' || choice == 'r' || choice == 'b' || choice == 'n') {
                board[new_r][new_c] = user ? choice : choice - 32;
                board[r][c] = '.';
                return &board[new_r][new_c];
            } else {
                printf("Invalid choice. Please choose again (q/r/b/n): ");
            }
        }
    }

    //Check the destination
    char dest_piece = board[new_r][new_c];
    if (user && (dest_piece >= 'a' && dest_piece <= 'z')) {
        return NULL;
    }
    if (!user && (dest_piece >= 'A' && dest_piece <= 'Z')) {
        return NULL;
    }
    //Move the pawn and capture if necessary
    board[new_r][new_c] = piece;
    board[r][c] = '.';
    return &board[new_r][new_c];
}

char* knight_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user) {
    //check if it is a knight
    char piece = board[r][c];
    if ((user && piece != 'n') || (!user && piece != 'N')) {
        return NULL;
    }
    //check if moved
    if (r == new_r && c == new_c) {
        return NULL;
    }
    //L shape
    int row = abs(new_r - r);
    int col = abs(new_c - c);
    if (!((row == 1 && col == 2) || (row == 2 && col == 1))) {
        return NULL;
    }
    //Check the destination
    char dest_piece = board[new_r][new_c];
    if (user && (dest_piece >= 'a' && dest_piece <= 'z')) {
        return NULL;
    }
    if (!user && (dest_piece >= 'A' && dest_piece <= 'Z')) {
        return NULL;
    }

    //Move the knight and capture if necessary
    board[new_r][new_c] = piece;
    board[r][c] = '.';

    return &board[new_r][new_c];
}
char* king_move(char board[SIZE][SIZE], int r, int c, int new_r, int new_c, bool user) {
    //check if it is a king
    char piece = board[r][c];
    if ((user && piece != 'k') || (!user && piece != 'K')) {
        return NULL;
    }
    //check if moved
    if (r == new_r && c == new_c) {
        return NULL;
    }
    //moving
    int row = abs(new_r - r);
    int col = abs(new_c - c);
    if (!((row == 1 || row == 0) && (col == 1 || col == 0))) {
        return NULL;
    }
    //Check the destination
    char dest_piece = board[new_r][new_c];
    if (user && (dest_piece >= 'a' && dest_piece <= 'z')) {
        return NULL;
    }
    if (!user && (dest_piece >= 'A' && dest_piece <= 'Z')) {
        return NULL;
    }
    // Move the pawn and capture if necessary
    board[new_r][new_c] = piece;
    board[r][c] = '.';

    return &board[new_r][new_c];
}