# duck-soup-nav

duck-soup-nav is a project that finds the shortest path between two points of longitude and latitude using data for road networks. It uses the A* searching algorithm to find the special route and a RTree to find the nearest node to a pair of coordinates. I'll explain the algorithmic decisions I made more in depth as the project progresses. Eventually, I hope to make this an api where duck-soup-nav takes https post requests and returns the path.

### Build

duck-soup-nav has no dependencies, so you can either compile all of the files together or use the makefile. To use the makefile, run the following command to build. It will create the target binary in `build/bin/ducksoup`

`$ make all`

Run `$ make help` to check out the other things to do with the Makefile!

### Run

To be able to run this, you must provide the road network data as .txt files. For my development so far, I am using Calfornia roads. Essentially, the nodes text file must have a new line for each node where the node id, longitude and latitude are separated by a space. The node is must start at 0 and increment by one each line. [This](https://www.cs.utah.edu/~lifeifei/research/tpq/cal.cnode) is that particular file I am using for nodes right now. [Here](https://www.cs.utah.edu/~lifeifei/research/tpq/cal.cedge) is the file for edges.

To be able to run this, you must have a file called `duck.cnf` set up. There is an example in this repository that you can check out. Essentially, here you specify the paths to network data and networking configuration (among other things in the future). It is a simple key-value where each pair is on a new line and is seperated by whitespace. The character `#` indicates a comment, and the line will not be intepreted.

`$ make run`

Now the server will be running under the address and port you specified in `duck.cnf`!

Also, unfortunately, things are a bit slow right now if they points to compute the route between are far away! Computing A* is not easy, and I have not experimented with finding a good heuristic yet or possibly using a different algorithm than A* search. Optimizations will come soon! :)

### Next plans

* Make the r-tree self balancing (this will reduce the height of the tree from over 200 to around 8)
* Find a differents routing algorithm that perhaps use a cross pollination of A* and other algorithms. (Think using the r-tree)
* Try different A* heuristics

### Usage

To use the basic routing algorithm, make a post request at the route `/route`. Pass into the body an json object with keys `"start"` and `"destination"` that both correspond to an array of length two that contains longitude and latitude at index 0 and 1 respectively. The response will be an array of length 2 arrays that contain longitude and latitude at index 0 and 1.

### Credit

Special thanks to the [researchers](https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm) at University of Utah that provided the database for the road networks. Their creation is awesome!

### Third party libraries and licenses
ducksoup is licensed under the [Mit License](https://opensource.org/licenses/MIT). [nlohmann/json](https://github.com/nlohmann/json) is licensed under the [Mit License](https://opensource.org/licenses/MIT). [yhirose/cpp-httplib](https://github.com/yhirose/cpp-httplib) is licensed under the [Mit License](https://opensource.org/licenses/MIT)