#include <iostream>
#include <vector>
#include <regex>

using namespace std;

const int MINIMAX_DEPTH = 4; 

void flip(char(&board)[8][8], int row, int col, char player)
{
    vector<vector<int>> discs_to_flip;

    char otherPlayer = (player == 'b') ? 'w' : 'b';

    int surroundingPosDeltas[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, // 3 positions above
                                      {0, -1}, {0, 1}, // 2 positions on same row
                                      {1, -1}, {1, 0}, {1, 1} }; // 3 positions below

    for (auto deltas : surroundingPosDeltas)
    {
        int curr_row = row + deltas[0];
        int curr_col = col + deltas[1];

        if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
            continue;

        char char_in_pos = board[curr_row][curr_col];

        bool flip_this_direction = false;

        if (char_in_pos == otherPlayer)
        {
            while (char_in_pos == otherPlayer)
            {
                curr_row += deltas[0];
                curr_col += deltas[1];

                if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
                    break;

                char_in_pos = board[curr_row][curr_col];
            }

            if (char_in_pos == player)
                flip_this_direction = true;

            if (flip_this_direction)
            {
                curr_row = row + deltas[0];
                curr_col = col + deltas[1];
                char_in_pos = board[curr_row][curr_col];

                while (char_in_pos == otherPlayer)
                {
                    vector<int> disc = { curr_row, curr_col };
                    discs_to_flip.push_back(disc);
                    curr_row += deltas[0];
                    curr_col += deltas[1];

                    char_in_pos = board[curr_row][curr_col];
                }

            }
        }
    }
    for (auto pos : discs_to_flip)
        board[pos[0]][pos[1]] = player;
}
bool isFlippable(char board[8][8], int row, int col, char player)
{
    char otherPlayer = (player == 'b') ? 'w' : 'b';

    int surroundingPosDeltas[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, // 3 positions above
                                      {0, -1}, {0, 1}, // 2 positions on same row
                                      {1, -1}, {1, 0}, {1, 1} }; // 3 positions below

    for (auto deltas : surroundingPosDeltas)
    {
        if (row + deltas[0] > 7 || row + deltas[0] < 0 || col + deltas[1] > 7 || col + deltas[1] < 0)
            continue;

        char char_in_pos = board[row + deltas[0]][col + deltas[1]]; 

        if (char_in_pos == otherPlayer)
        {
            int curr_row = row + deltas[0];
            int curr_col = col + deltas[1];

            while (char_in_pos == otherPlayer)
            {
                curr_row += deltas[0];
                curr_col += deltas[1];

                if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_row < 0)
                    break;

                char_in_pos = board[curr_row][curr_col];
            }

            if (char_in_pos == player)
                return true;
        }
    }
    return false;
}
void makeMove(char(&board)[8][8], int row, int col, char player) 
{
    board[row][col] = player;

    flip(board, row, col, player);
}
vector<vector<int>> calculateLegalMoves(char board[8][8], char player)
{
    vector<vector<int>> move_list;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (board[i][j] == '-')
                if (isFlippable(board, i, j, player))
                {
                    vector<int> move = { i, j };
                    move_list.push_back(move);
                }
    return move_list;
}
bool isLegalMove(char board[8][8], vector<vector<int>> move_list, int row, int col, char player)
{
    vector<int> proposedMove = { row, col };

    if (row > 7 || row < 0 || col > 7 || col < 0)
        throw range_error{ "isLegalMove()" };

    if (board[row][col] != '-')
    {
        return false;
    }
    if (find(move_list.begin(), move_list.end(), proposedMove) != move_list.end())
    {
        return true;
    }

    return false;
}
vector<vector<int>> getBlackLegalMoves(char board[8][8])
{
    return calculateLegalMoves(board, 'b');
}
vector<vector<int>> getWhiteLegalMoves(char board[8][8])
{
    return calculateLegalMoves(board, 'w');
}
void printLegalMoves(char board[8][8], char player)
{
    if (player == 'b')
    {
        cout << "Black legal moves:\n";
        auto v = getBlackLegalMoves(board);
        for (const auto& vec : v)
            cout << "(" << vec[0] << "," << vec[1] << ")  ";
        cout << endl;
    }
    else
    {
        cout << "White legal moves:\n";
        auto x = getWhiteLegalMoves(board);
        for (const auto& vec : x)
            cout << "(" << vec[0] << "," << vec[1] << ")  ";
        cout << endl;
    }
}
void printLegalMoves(vector<vector<int>> move_list)
{
    for (const auto& vec : move_list)
    {
        cout << "(" << vec[0] << "," << vec[1] << ")  ";
    }
    cout << endl;
}
ostream& operator<<(ostream& os, const char board[8][8])
{
    cout << "   0  1  2  3  4  5  6  7\n";
    for (int i = 0; i < 8; ++i)
    {
        cout << (i) << "  ";
        for (int j = 0; j < 8; ++j)
            cout << board[i][j] << "  ";
        cout << '\n';
    }
    return os;
}
bool isGameOver(char board[8][8])
{
    return getBlackLegalMoves(board).empty() && getWhiteLegalMoves(board).empty();
}
int getScore(char board[8][8], char player)
{
    int total = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (board[i][j] == player)
                total += 1;
    return total;
}
void printWinner(char(&board)[8][8])
{
    int white_total = getScore(board, 'w');
    int black_total = getScore(board, 'b');

    cout << "Black total: " << black_total << '\n';
    cout << "White total: " << white_total << '\n';
    if (black_total == white_total)
    {
        cout << "TIE GAME\n";
        return;
    }
    cout << ((black_total > white_total) ? "Black" : "White") << " wins!\n";
}
int heuristic(char board[8][8])
{
    int b_total = 0;
    int w_total = 0;

    b_total += getBlackLegalMoves(board).size();
    w_total += getWhiteLegalMoves(board).size();

    b_total += getScore(board, 'b');
    w_total += getScore(board, 'w');

    if (board[0][0] == 'w') 
        w_total += 10;

    if (board[0][0] == 'b') 
        b_total += 10;

    if (board[7][0] == 'w') 
        w_total += 10;
   
    if (board[7][0] == 'b') 
        b_total += 10;
   
    if (board[0][7] == 'w') 
        w_total += 10;
    
    if (board[0][7] == 'b') 
        b_total += 10;
    
    if (board[7][7] == 'w') 
        w_total += 10;

    if (board[7][7] == 'b') 
        b_total += 10;

    return (b_total - w_total);
}
struct Node
{
    Node** children;
    int child_count;
    vector<vector<int>> move_list;
    char state[8][8];
    int val;
};
Node* CreateTree(char board[8][8], int depth, char player)
{
    Node* node = new Node();

    node->move_list = (player == 'w') ? getWhiteLegalMoves(board) : getBlackLegalMoves(board);

    node->child_count = node->move_list.size();

    memcpy(node->state, board, 8 * 8 * sizeof(char));

    char other_player = (player == 'w') ? 'b' : 'w';

    if (depth > 0 && node->child_count > 0)
    {
        node->children = new Node * [node->child_count];

        for (int i = 0; i < node->child_count; ++i)
        {
            char tmp_board[8][8];
            memcpy(tmp_board, board, 8 * 8 * sizeof(char));

            makeMove(tmp_board, node->move_list[i][0], node->move_list[i][1], player);

            node->children[i] = CreateTree(tmp_board, depth - 1, other_player);
        }
    }
    else
        node->children = NULL;

    return node;
}
int minimax(Node* position, int depth, int alpha, int beta, bool maximizing_player)
{
    if (depth == 0 || isGameOver(position->state))
        return heuristic(position->state);

    if (maximizing_player)
    {
        int max_eval = -9999999; 

        for (int i = 0; i < position->child_count; ++i)
        {
            int eval = minimax(position->children[i], depth - 1, alpha, beta, false);
            max_eval = max(max_eval, eval); 

            alpha = max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        position->val = max_eval; 
        return max_eval;
    }
    else
    { 
        int min_eval = 9999999; 
        for (int i = 0; i < position->child_count; ++i)
        {
            int eval = minimax(position->children[i], depth - 1, alpha, beta, true);
            min_eval = min(min_eval, eval); 

            beta = min(beta, eval);
            if (beta <= alpha)
                break;
        }
        position->val = min_eval; 
        return min_eval;
    }
}
int minimax(Node* position, int depth, bool maximizing_player)
{
    if (depth == 0 || isGameOver(position->state))
        return heuristic(position->state);

    if (maximizing_player) 
    {
        int max_eval = -9999999;
        for (int i = 0; i < position->child_count; ++i) 
        {
            int eval = minimax(position->children[i], depth - 1, false);
            max_eval = max(max_eval, eval);
        }
        position->val = max_eval;
        return max_eval;
    }
    else 
    {
        int min_eval = 9999999;
        for (int i = 0; i < position->child_count; ++i) 
        {
            int eval = minimax(position->children[i], depth - 1, true);
            min_eval = min(min_eval, eval);
        }
        position->val = min_eval;
        return min_eval;
    }
}
int main()
{
    //Initialize Game Board
    char board[8][8];
    for (auto& i : board) {
        for (char& j : i) {
            j = '-';
        }
    }

    board[3][3] = 'w'; board[3][4] = 'b';
    board[4][3] = 'b'; board[4][4] = 'w';

    int total_moves = 0;
    char player = 'b'; // black always goes first

    regex move_input_pattern("[0-7] [0-7]"); 
    regex player_selection_pattern("w|b"); 
    cout << "\n";
    cout << "  ||||||||||  |||||||||||  |||    |||  ||||||||||  |||      |||      ||||||||||\n";
    cout << "  |||    |||      |||      |||    |||  |||         |||      |||      |||    |||\n";
    cout << "  |||    |||      |||      ||||||||||  ||||||||||  |||      |||      |||    |||\n";
    cout << "  |||    |||      |||      |||    |||  |||         |||      |||      |||    |||\n";
    cout << "  ||||||||||      |||      |||    |||  ||||||||||  |||||||  |||||||  ||||||||||\n\n";
    cout << "               ||||||||||  |||||||||||  |||       |||  ||||||||||\n";
    cout << "               |||         |||     |||  ||||     ||||  |||       \n";
    cout << "               |||  |||||  |||||||||||  |||||   |||||  ||||||||||\n";
    cout << "               |||    |||  |||     |||  ||| ||||| |||  |||       \n";
    cout << "               ||||||||||  |||     |||  |||  |||  |||  ||||||||||\n\n";
    cout << "Which colour do you want? Black or White?\nEnter 'b' to play as black or 'w' to play as white: ";
    string selected_player;
    
    while (true)
    {
        getline(cin, selected_player);
        if (!regex_match(selected_player, player_selection_pattern))
        {
            cout << "\nInvalid input: \"Enter 'b' to be black or 'w' to be white. \n";
            continue;
        }
        break;
    }

    char player_char = selected_player[0];
    cout << "You have chosen to play as " << ((player_char == 'w') ? "white" : "black") << "!\n\n";

    char ai_char = ((player_char == 'w') ? 'b' : 'w');

    // start game
    while (!isGameOver(board))
    {
        vector<vector<int>> move_list = calculateLegalMoves(board, player);

        if (player == 'b' && getBlackLegalMoves(board).empty())
        {
            cout << "Black is out of moves, PASS to White.\n";
            player = 'w';
            continue;
        }
        if (player == 'w' && getWhiteLegalMoves(board).empty())
        {
            cout << "White is out of moves, PASS to Black.\n";
            player = 'b';
            continue;
        }

        int white_total = getScore(board, 'w');
        int black_total = getScore(board, 'b');

        cout << "Black total: " << black_total << '\n';
        cout << "White total: " << white_total << '\n';

        cout << board; // show board
        cout << '\n';

        // Player turn
        if (player == player_char)
        {
            printLegalMoves(board, player_char);
            string user_input;
            while (true)
            {
                cout << ((player == 'w') ? "Your move (w): " : "Your move (b): ");
                getline(cin, user_input);

                if (!regex_match(user_input, move_input_pattern))
                {
                    cout << "\nInvalid input :((\n";
                    continue;
                }
                else
                {
                    int row = user_input[0] - '0';
                    int col = user_input[2] - '0';

                    try
                    {
                        if (isLegalMove(board, move_list, row, col, player))
                            makeMove(board, row, col, player);
                        else
                        {
                            cout << "Illegal move! Try again.\n";
                            continue;
                        }
                    }
                    catch (range_error& e)
                    {
                        cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
                        return 1;
                    }
                    break;
                }
            }
        }
        else // AI turn
        {
            auto gametree = CreateTree(board, MINIMAX_DEPTH, player); 
            bool maximizer = (player == 'b') ? true : false;

            int optimial_val = minimax(gametree, MINIMAX_DEPTH, -99999999, 99999999, maximizer);

            for (int i = 0; i < gametree->child_count; ++i)
                if (gametree->children[i]->val == optimial_val)
                {
                    bool same_config = true;
                    for (int j = 0; j < 7; ++j)
                        for (int k = 0; k < 7; ++k)
                        {
                            if (gametree->children[i]->state[j][k] != board[j][k])
                                same_config = false;
                        }

                    if (!same_config)
                        memcpy(board, gametree->children[i]->state, 8 * 8 * sizeof(char));
                    else 
                        makeMove(board, move_list[0][0], move_list[0][1], player);
                    break;
                }
        }
        total_moves += 1;
        // Switch players
        player = (player == 'w') ? 'b' : 'w';
    }
    cout << board; // Show final board
    printWinner(board);
    return 0;
}