# pond-solver
A solver for the mobile game "Pond" (浅塘）

You can via download the game (free) via https://www.taptap.com/app/8708. Instructions are also available there.

This solver utilizes Dijkstra algorithm to find the smallest number of steps to escape the Pond. Note there can be multiple win states (destination in Dijkstra's term) in the game of Pond. The solver stops when it finds a win state that yields the smallest number of steps.

Some optimizations are made to make the search fast, including:
 * skipping impossible moves early.

The solver also supports a simulation mode to replay the input moves. This can be used to verify the solver result itself.
