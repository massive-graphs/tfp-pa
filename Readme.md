TFP-GraphGenerator
==================

This repository contains preferential attachment graph generators able to produce
graphs not fitting into main memory. The underlying idea is introduced in
"Generating Massive Scale-Free Networks under Resource Constraints", U.Meyer
and M.Penschuck, ALENEX16. This implementation, however, uses vertex-based tokens
rather edge based, which better fits the POT-constraints imposed by the STXXL.

Building
--------
This generators make heavy use of STXXL <http://stxxl.sourceforge.net/> and requires
a version NEWER than 1.4. If it is not yet installed you can do using the following
commands:

    cd {INSTALLATION-DIR}
    git clone https://github.com/stxxl/stxxl
    cd stxxl
    mkdir build
    cd build
    cmake ..
    make -j 4

Afterwards set the environment variable STXXL_DIR="{INSTALLATION-DIR}/stxxl/build", e.g.
by adding

    export STXXL_DIR="{INSTALLATION-DIR}/stxxl/build"

to your .bashrc / .profile (or similar) and reloading your shell.

If you want to build the test suite GTest has to be installed and the environment
variables have to be set accordingly.

In order to build the graph generators navigate into its root directory and execute:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make -j 4

Edge-List-Format
----------------
All generators produce a binary edge vectors which fully represent the graphs.
By default each vertex is represented by a 64-bit unsigned integer, however smaller
data types (32, 40, 48) are supported. Just edit the "-DFILE_DATA_WIDTH=" parameter
in CMakeLists.txt and recompile; the 40-bit / 48-bit types correspond to the STXXL::uintX
types.

The files does not have a header but only contains the binary vector ids. Two
consecutive entries for an edge (from, to). In an undirected graph only one edge
with (from < to) is stored.

Barabasi-Albert-Generator
-------------------------
Usage: ./tfp_ba [options] <filename> <no-vertices> <edges-per-vert>

The Barabasi-Albert model yields undirected scale-free graphs. Given a small seed graph
(in our case a ring with 2*<edges-per-vert> vertices) a edge is introduce and connected
to <edges-per-vert> existing vertices. This procedure is repeated until <no-vertices>
vertices are added. We allow multi-edges during this process (use -m to later remove them;
this will however affect the average degree in the graph).

During the sampling of the <edges-per-vert> neighbors for vertex there are two modes of
operation: By default, all neighbors are sampled "in parallel" with the same probability
distribution. Use the -d option, to sample the neighbors "sequentially", i.e. to take
the shifted probability distribution of earlier edges into account. In this mode, self-loops
are possible, which can be later removed using the -s option.

Directed PA by B Bollobas, C. Borgs, J. Chayes, O. Riordan
----------------------------------------------------------
Usage: ./tfp_bbcr [options] <filename> <no-edges>

This model again starts with a small seed ring of <seed-vertices, default 100> vertices.
Then parameters alpha + beta + gamma = 1 are used to select one of the following methods
to introduce a new edge:
- With prob. alpha add a new vertex and connect with an out-going edge; 
  the neighbor is sampled according to its in-degree
- With prob. beta two existing vertices u, v are connected with edge (u, v) where
  u is sampled based on its out-degree and v based on its in-degree
- With prob. gamma add a new vertex and connect with an in-coming edge;
  the neighbor is sampled according to its out-degree

The probability to select a vertex u based on its out-degree is proportional to
   Pout[u] \propto deg_out(u) + <d-out>
where <d-out, default 0.0> is a constant. The higher <d-out> the more uniform the sampling
takes place (i.e. the preferential attachment is damped). The mechanism for in-degree sampling
is analogous.

Computing the Degree Distribution
---------------------------------
This suite also include a distribution counter:

    # undirected graphs
    ./tfp_ba /local/graph.bin 1m 10
    ./distribution_count -o /local/distr /local/graph.bin
    gnuplot -e "datafile='/local/distr'" ../gnuplots/undirected_degree.gp

    # directed graphs
    ./tfp_bbcr /local/graph.bin 10m
    ./distribution_count --directed -o /local/distr /local/graph.bin
    gnuplot -e "datafile='/local/distr'" ../gnuplots/directed_degree.gp



