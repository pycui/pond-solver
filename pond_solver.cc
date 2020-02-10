#include <iostream>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::unordered_set;
using std::vector;

namespace {
constexpr const int kBoardDimension = 6;
}

enum class Direction { VERTICAL = 0, HORIZONTAL = 1 };

struct Piece {
  Piece(Direction direction, int size, int fix_axis_pos,
        int moving_axis_init_pos)
      : direction(direction), size(size), fix_axis_pos(fix_axis_pos) {
    moving_axis_pos = moving_axis_init_pos;
  }

  const Direction direction;
  const int size;
  const int fix_axis_pos;
  int moving_axis_pos;
};

class Board {
public:
  Board() = default;
  void AddPiece(const Piece piece) { pieces_.push_back(piece); }
  bool Win() { return pieces_[0].moving_axis_pos == kBoardDimension - 2; }
  bool CanMove(int i, int new_pos) {
    return pieces_[i].size() + new_pos <= kBoardDimension;
  }

  bool MovePiece(int i, int new_pos) {
    int old_pos = pieces_[i].moving_axis_pos;
    pieces_[i].moving_axis_pos = new_pos;
    if (IsVisited(GetBoardState())) {
      pieces_[i].moving_axis_pos = old_pos;
      return false;
    }
    visited_.insert(GetBoardState());
    if (!IsValid()) {
      pieces_[i].moving_axis_pos = old_pos;
      return false;
    }
    return true;
  }
  bool IsValid() {
    for (int i = 0; i < pieces_.size(); i++) {
      for (int j = 0; j < pieces_.size(); j++) {
        unordered_set<int> i_occurrencies;
        // Fill i's occurrencies.
        if (pieces_[i].direction == Direction::VERTICAL) {
          for (int k = pieces_[i].moving_axis_pos;
               k < pieces_[i].moving_axis_pos + pieces_[i].size; k++) {
            int state = (pieces_[i].fix_axis_pos << 16) + k;
            i_occurrencies.insert(state);
          }
        } else {
          for (int k = pieces_[i].moving_axis_pos; k < pieces_[i].moving_axis_pos + pieces_[i].size; k++) {
            int state = (k << 16) + pieces_[i].fix_axis_pos;
            i_occurrencies.insert(state);
          }
        }

        // Check each of j's occupancy.
        if (pieces_[j].direction == Direction::VERTICAL) {
          for (int k = pieces_[j].moving_axis_pos; k < pieces_[j].moving_axis_pos + pieces_[j].size; k++) {
            int state = (pieces_[j].fix_axis_pos << 16) + k;
            if (i_occurrencies.count(state)) {
              cout << "Piece " << j << " will be overlapped with Piece " << i << endl;
              return false;
            }
          }
        } else {
          for (int k = pieces_[j].moving_axis_pos; pieces_[j].moving_axis_pos + pieces_[j].size; k++) {
            int state = (k << 16) + pieces_[j].fix_axis_pos;
            if (i_occurrencies.count(state)) {
              cout << "Piece " << j << " will be overlapped with Piece " << i << endl;
              return false;
            }
          }
        }
      }
    }
    return true;
  }

  long long int GetBoardState() {
    long long int state = 0;
    for (int i = 0; i < pieces_.size(); i++) {
      state += pieces_[i].moving_axis_pos << (i * 3);
    }
    return state;
  }
  bool IsVisited(long long int state) {
    return visited_.count(state);
  }

  bool Solve() {
    if (Win()) {
      cout << "Found solution!" << endl;
      PrintSolution();
      return true;
    }
    for (int i = 0; i < pieces_.size; i++) {
      for (int j = 0; j < kBoardDimension; j++) {
        if (MovePiece(i, j)) {
          steps.push_back(pair<int, int>(i, j));
          if (Solve()) {
            return true;
          }
          steps.pop_back();
        }
      }
    }
    return false;
  }
  int NumPieces() { return pieces_.size(); }
  void PrintSolution() {
    for (int i = 0; i < steps.size(); i++) {
      cout << "Step " << i << ": " << steps[i].first << " " << steps[i].second
           << endl;
    }
    cout << "Solution printing completed" << endl;
  }

private:
  vector<Piece> pieces_;
  unordered_set<long long int> visited_;
  vector<pair<int, int>> steps;
};

Direction GetDirection(string direction_str) {
  if (direction_str == 'v') {
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
    board.AddPiece(Piece(GetDirection(direction), size, fix_axis_pos,
                         moving_axis_init_pos));
  }
  cout << "Piece entering done. Do you want the solver to solve? Y for solver, "
          "otherwise it will enter simulation mode."
       << endl;
  bool auto_solve;
  if (!auto_solve) {
    cout << "Now start playing simulation, by (piece_index, new_pos)" << endl;
    int piece_index;
    int new_pos;
    while (cin >> piece_index >> new_pos) {
      if (!board.CanMove(piece_index, new_pos)) {
        cout << "Can't make this move. Try again..." << endl;
        continue;
      }
      if (!board.MovePiece(piece_index, new_pos)) {
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
    board.Solve();
  }
  return 0;
}
