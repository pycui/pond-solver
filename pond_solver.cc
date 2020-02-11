#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::priority_queue;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using BoardState = long long int;

namespace {
constexpr const int kBoardDimension = 6;
}

enum class Direction { VERTICAL = 0, HORIZONTAL = 1 };

enum class MoveStatus { OUT_OF_BOUNDARY = 0, OVERLAPPED = 1, VISITED = 2, OK = 3 };

struct Piece {
  Piece(Direction direction, int size, int fix_axis_pos, int moving_axis_init_pos)
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

  MoveStatus MovePiece(int i, int new_pos, BoardState previous_state, int current_steps) {
    if (pieces_[i].size + new_pos > kBoardDimension) {
      return MoveStatus::OUT_OF_BOUNDARY;
    }
    int old_pos = pieces_[i].moving_axis_pos;
    pieces_[i].moving_axis_pos = new_pos;
    if (!IsValid(i)) {
      pieces_[i].moving_axis_pos = old_pos;
      return MoveStatus::OVERLAPPED;
    }
    if (simulation_mode_) {
      return MoveStatus::OK;
    }
    int state = GetBoardState();
    // int current_steps = GetStepsToOrigin(previous_state) + 1;
    // int current_steps = dist_[state] + 1;
    if (IsVisited(state)) {
      // if (current_steps < dist_[state]) {
      //   prev_state_of_state_[state] =
      //       pair<BoardState, pair<int, int>>(previous_state, pair<int, int>(i, new_pos));
      //   dist_[state] = current_steps;
      // }
      pieces_[i].moving_axis_pos = old_pos;
      return MoveStatus::VISITED;
    }
    // visited_.insert(state);
    prev_state_of_state_[state] =
        pair<BoardState, pair<int, int>>(previous_state, pair<int, int>(i, new_pos));
    // dist_[state] = current_steps;
    return MoveStatus::OK;
  }

  bool IsValid(int moved_index) {
    for (int i = 0; i < pieces_.size(); i++) {
      if (i == moved_index) {
        continue;
      }
      unordered_set<int> i_occurrencies;
      // Fill i's occurrencies.
      if (pieces_[i].direction == Direction::VERTICAL) {
        for (int k = pieces_[i].moving_axis_pos; k < pieces_[i].moving_axis_pos + pieces_[i].size;
             k++) {
          int state = (pieces_[i].fix_axis_pos << 16) + k;
          i_occurrencies.insert(state);
        }
      } else {
        for (int k = pieces_[i].moving_axis_pos; k < pieces_[i].moving_axis_pos + pieces_[i].size;
             k++) {
          int state = (k << 16) + pieces_[i].fix_axis_pos;
          i_occurrencies.insert(state);
        }
      }
      int j = moved_index;
      // Check each of j's occupancy.
      if (pieces_[j].direction == Direction::VERTICAL) {
        for (int k = pieces_[j].moving_axis_pos; k < pieces_[j].moving_axis_pos + pieces_[j].size;
             k++) {
          int state = (pieces_[j].fix_axis_pos << 16) + k;
          if (i_occurrencies.count(state)) {
            return false;
          }
        }
      } else {
        for (int k = pieces_[j].moving_axis_pos; k < pieces_[j].moving_axis_pos + pieces_[j].size;
             k++) {
          int state = (k << 16) + pieces_[j].fix_axis_pos;
          if (i_occurrencies.count(state)) {
            return false;
          }
        }
      }
    }
    return true;
  }

  BoardState GetBoardState() {
    BoardState state = 0;
    for (int i = 0; i < pieces_.size(); i++) {
      state += pieces_[i].moving_axis_pos << (i * 3);
    }
    return state;
  }
  void ResumeState(BoardState state) {
    for (int i = 0; i < pieces_.size(); i++) {
      pieces_[i].moving_axis_pos = (state >> (i * 3)) % 8;
    }
  }
  bool IsVisited(BoardState state) { return prev_state_of_state_.count(state); }

  void SolveAll() {
    // Save initial states.
    BoardState initial_state = GetBoardState();
    // visited_.insert(initial_state);
    prev_state_of_state_[initial_state] =
        pair<BoardState, pair<int, int>>(-1, pair<int, int>(-1, -1));
    dist_[initial_state] = 0;
    queue_.push(std::make_pair(0, initial_state));
    int result = Solve();
    if (result != -1) {
      cout << "Win!" << endl;
      // cout << "Found " << win_states_.size() << " solutions." << endl;
      // int min_steps = 100000000;
      // BoardState min_state = -1;
      // for (BoardState win_state : win_states_) {
      //   int current_steps = GetStepsToOrigin(win_state);
      //   if (current_steps < min_steps) {
      //     min_steps = current_steps;
      //     min_state = win_state;
      //   }
      // }
      BoardState state = win_state_;
      while (prev_state_of_state_[state].first != -1) {
        cout << prev_state_of_state_[state].second.first << ' '
             << prev_state_of_state_[state].second.second << endl;
        state = prev_state_of_state_[state].first;
      }
      cout << "Best solution has " << result << " steps." << endl;
    } else {
      cout << "Could not find a solution." << endl;
    }
  }

  int Solve() {
    while (!queue_.empty()) {
      int current_steps;
      BoardState current_state;
      std::tie(current_steps, current_state) = queue_.top();
      cout << "Solve now..." << current_steps << ' ' << current_state << endl;
      queue_.pop();
      ResumeState(current_state);
      if (Win()) {
        win_state_ = current_state;
        return current_steps;
      }
      // BoardState prev_state = GetBoardState();
      int count = 0;
      for (int i = 0; i < pieces_.size(); i++) {
        int old_pos = pieces_[i].moving_axis_pos;
        // up/left side
        for (int j = old_pos - 1; j >= 0; j--) {
          MoveStatus status = MovePiece(i, j, current_state, current_steps + 1);
          if (status == MoveStatus::OUT_OF_BOUNDARY || status == MoveStatus::OVERLAPPED) {
            break;
          }
          if (status == MoveStatus::OK) {
            count++;
            queue_.push(std::make_pair(current_steps + 1, GetBoardState()));
            // int result = Solve();
            // if (result != -1) {
            //   return result;
            // }
            // pieces_[i].moving_axis_pos = old_pos;
          } // skipped status == MoveStatus::VISITED
        }
        // down/right side
        for (int j = old_pos + 1; j < kBoardDimension; j++) {
          MoveStatus status = MovePiece(i, j, current_state, current_steps + 1);
          if (status == MoveStatus::OUT_OF_BOUNDARY || status == MoveStatus::OVERLAPPED) {
            break;
          }
          if (status == MoveStatus::OK) {
            count++;
            queue_.push(std::make_pair(current_steps + 1, GetBoardState()));
            // int result = Solve();
            // if (result != -1) {
            //   return result;
            // }
            // pieces_[i].moving_axis_pos = old_pos;
          }
        }
      }
      cout << "count = " << count << endl;
    }
    return -1;
  }

  int NumPieces() { return pieces_.size(); }

  void PrintSolution() { cout << "TODO" << endl; }

  void SetSimulationMode() { simulation_mode_ = true; }

  // int GetStepsToOrigin(BoardState state) {
  //   int current_steps = 0;
  //   while (prev_state_of_state_[state].first != -1) {
  //     state = prev_state_of_state_[state].first;
  //     current_steps++;
  //   }
  //   return current_steps;
  // }

private:
  vector<Piece> pieces_;
  //  unordered_set<BoardState> visited_;
  unordered_map<BoardState, pair<BoardState, pair<int, int>>>
      prev_state_of_state_; // current_state -> previous_state, piece_index, new_pos.
  // bool has_won_ = false;
  // unordered_set<BoardState> win_states_;
  priority_queue<pair<int, BoardState>, vector<pair<int, BoardState>>,
                 std::greater<pair<int, BoardState>>>
      queue_;
  // unordered_map<BoardState, int> dist_;
  bool simulation_mode_ = false;
  BoardState win_state_ = -1;
};

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
