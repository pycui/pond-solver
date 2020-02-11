#include "pond_solver.h"

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

void Board::AddPiece(const Piece piece) { pieces_.push_back(piece); }

bool Board::Win() { return pieces_[0].moving_axis_pos == kBoardDimension - 2; }

MoveStatus Board::MovePiece(int i, int new_pos, BoardState previous_state, int current_steps) {
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
  if (IsVisited(state)) {
    pieces_[i].moving_axis_pos = old_pos;
    return MoveStatus::VISITED;
  }
  prev_state_of_state_[state] =
      pair<BoardState, pair<int, int>>(previous_state, pair<int, int>(i, new_pos));
  return MoveStatus::OK;
}

bool Board::IsValid(int moved_index) {
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

BoardState Board::GetBoardState() {
  BoardState state = 0;
  for (int i = 0; i < pieces_.size(); i++) {
    state += pieces_[i].moving_axis_pos << (i * 3);
  }
  return state;
}
void Board::ResumeState(BoardState state) {
  for (int i = 0; i < pieces_.size(); i++) {
    pieces_[i].moving_axis_pos = (state >> (i * 3)) % 8;
  }
}
bool Board::IsVisited(BoardState state) { return prev_state_of_state_.count(state); }

void Board::SolveAll() {
  // Save initial states.
  BoardState initial_state = GetBoardState();
  prev_state_of_state_[initial_state] =
      pair<BoardState, pair<int, int>>(-1, pair<int, int>(-1, -1));
  queue_.push(std::make_pair(0, initial_state));
  int result = Solve();
  if (result != -1) {
    cout << "Win! " << result << endl;
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

int Board::Solve() {
  while (!queue_.empty()) {
    int current_steps;
    BoardState current_state;
    std::tie(current_steps, current_state) = queue_.top();
    queue_.pop();
    ResumeState(current_state);
    if (Win()) {
      win_state_ = current_state;
      return current_steps;
    }
    for (int i = 0; i < pieces_.size(); i++) {
      int old_pos = pieces_[i].moving_axis_pos;
      int count = 0;
      int last_move = -1;
      // up/left side
      for (int j = old_pos - 1; j >= 0; j--) {
        MoveStatus status = MovePiece(i, j, current_state, current_steps + 1);
        if (status == MoveStatus::OUT_OF_BOUNDARY || status == MoveStatus::OVERLAPPED) {
          break;
        }
        if (status == MoveStatus::VISITED) {
          continue;
        }
        queue_.push(std::make_pair(current_steps + 1, GetBoardState()));
        count++;
        last_move = j;
        pieces_[i].moving_axis_pos = old_pos;
      }
      // down/right side
      for (int j = old_pos + 1; j < kBoardDimension; j++) {
        MoveStatus status = MovePiece(i, j, current_state, current_steps + 1);
        if (status == MoveStatus::OUT_OF_BOUNDARY || status == MoveStatus::OVERLAPPED) {
          break;
        }
        if (status == MoveStatus::VISITED) {
          continue;
        }
        queue_.push(std::make_pair(current_steps + 1, GetBoardState()));
        count++;
        last_move = j;
        pieces_[i].moving_axis_pos = old_pos;
      }
      if (count) {
        cout << "Moved " << i << " with " << count << " different options. Last move to "
             << last_move << " its old_pos: " << old_pos << endl;
      }
    }
  }
  return -1;
}

void Board::PrintBoard() {
  //
}
