# pond-solver
A solver for the mobile game "Pond"

This solver utilies Depth-First-Search (DFS) to implement a Dijkstra-like algorithm. Unlikely traditional Dijkstra problem, there can be multiple winning states (destination in Dijkstra's term) in the game of Pond. The solver finds all possible winning states, and calculate shortest path for each winning state to choose a global best solution.

Some optimizations are made to make the search fast, including:
 * skipping impossible moves early.

The solver also supports a simulation mode to replay the input moves. This can be used to verify the solver result itself.
