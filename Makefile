# source files.
SRC =  $(addsuffix .cpp, \
	$(addprefix delaunay/, Edge EdgeList EdgeReorderer Functions Halfedge \
				HalfedgePriorityQueue ICoord kruskal LR Site SiteList Vertex Voronoi) \
	$(addprefix geom/, Circle LineSegment Point Rectangle Polygon Triangle) ) 

OBJ = $(SRC:.cpp=.o)
OUT = libcppdelaunay.a

# include directories
INCLUDES = -I. -I/usr/include

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -O0
# compiler
CCC = g++

# library paths
#LIBS = -L../ -L/usr/local/lib -lm

# compile flags
LDFLAGS = -g

.SUFFIXES: .cpp

all: $(OUT)

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT)