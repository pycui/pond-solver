# pond-solver
A solver for the mobile game "Pond"

For the game play and instructions, please see https://www.taptap.com/app/8708.

Example step of game play (from Tutorial)

This solver utilies Dijkstra algorithm to find the smallest number of steps to escape the Pond. Note there can be multiple winning states (destination in Dijkstra's term) in the game of Pond. The solver stops when it finds a winning state that yeilds the smallest number of steps.

Some optimizations are made to make the search fast, including:
 * skipping impossible moves early.

The solver also supports a simulation mode to replay the input moves. This can be used to verify the solver result itself.
