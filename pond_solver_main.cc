#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pond_solver.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

Direction GetDirection(string direction_str) {
  if (direction_str == "v") {
    return Direction::VERTICAL;
  } else {
    return Direction::HORIZONTAL;
  }
  // TODO: add error-handling.
}

int main() {
  cout << "Pond game started. Please enter the number of pieces" << endl;
  Board board;
  int num_pieces;
  cin >> num_pieces;
  cout << "Please add the pieces, by (diretion, size, fix_axis_pos, "
          "moving_axis_init_pos)"
       << endl;
  for (int i = 0; i < num_pieces; i++) {
    string direction;
    int size;
    int fix_axis_pos;
    int moving_axis_init_pos;
    cin >> direction >> size >> fix_axis_pos >> moving_axis_init_pos;
    board.AddPiece(Piece(GetDirection(direction), size, fix_axis_pos, moving_axis_init_pos));
  }
  cout << "Piece entering done. Do you want the solver to solve? Y for "
          "solver, "
          "otherwise it will enter simulation mode."
       << endl;
  string auto_solve;
  cin >> auto_solve;
  if (auto_solve != "y" && auto_solve != "Y") {
    cout << "Now start playing simulation, by (piece_index, new_pos)" << endl;
    board.SetSimulationMode();
    int piece_index;
    int new_pos;
    while (cin >> piece_index >> new_pos) {
      MoveStatus status = board.MovePiece(piece_index, new_pos, -1, 0);
      if (status == MoveStatus::OUT_OF_BOUNDARY) {
        cout << "Piece out of boundary. Try again..." << endl;
        continue;
      }
      if (status == MoveStatus::OVERLAPPED) {
        cout << "Piece overlapped. Try again..." << endl;
        continue;
      }
      if (board.Win()) {
        cout << "You win!" << endl;
        break;
      }
    }
  } else {
    cout << "Start auto solving..." << endl;
    board.SolveAll();
  }
  return 0;
}
