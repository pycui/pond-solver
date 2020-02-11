
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
  using BoardState = long long int;

  Board() = default;
  void AddPiece(const Piece piece);

  bool Win();

  MoveStatus MovePiece(int i, int new_pos, BoardState previous_state, int current_steps);

  bool IsValid(int moved_index);

  BoardState GetBoardState();

  void ResumeState(BoardState state);

  bool IsVisited(BoardState state);

  void SolveAll();

  int NumPieces() { return pieces_.size(); }

  void SetSimulationMode() { simulation_mode_ = true; }

  void PrintBoard();
private:
  std::vector<Piece> pieces_;
  std::unordered_map<BoardState, std::pair<BoardState, std::pair<int, int>>>
      prev_state_of_state_; // current_state -> previous_state, piece_index, new_pos.
  std::priority_queue<std::pair<int, BoardState>, std::vector<std::pair<int, BoardState>>,
                      std::greater<std::pair<int, BoardState>>>
      queue_;
  bool simulation_mode_ = false;
  BoardState win_state_ = -1;

  int Solve();
};
