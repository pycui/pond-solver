# pond-solver
A solver for the mobile game "Pond"

This solver utilies Dijkstra algorithm to find the smallest number of steps to escape the Pond. Note there can be multiple winning states (destination in Dijkstra's term) in the game of Pond. Also, the graph is constructed dynamically. The solver stops when it finds the winning state that yeilds the smallest number of steps.

Some optimizations are made to make the search fast, including:
 * skipping impossible moves early.

The solver also supports a simulation mode to replay the input moves. This can be used to verify the solver result itself.
