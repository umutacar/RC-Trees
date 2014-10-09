######################################################################
## Makefile
##
## Umut A. Acar
######################################################################

COMPILER = g++ -c -Wno-deprecated
LINKER = g++

#Define some flags
OPT_FLAG =  -O4 -finline-functions 
DEBUG_FLAGS = -g  #-pg 

# Optimized
CFLAGS = $(OPT_FLAG) -DNDEBUG -DNVERBOSE -g
# Debugging 
#CFLAGS = $(DEBUG_FLAGS) -Wall -Werror -g #-pedantic

LFLAGS = -g #-pg

CC = $(COMPILER) $(CFLAGS)   
LINK = $(COMPILER) $(LFLAGS)

OBJS = Application.o AMath.o Interface.o  BinCluster.o UnaryCluster.o FinalCluster.o Debug.o Data.o  FreeList.o Contract.o Queue.o PQueue.o Test.o Tree.o Vertex.o ClusterList.o

all: clean test 

Application.o: Application.c
	$(CC) -o Application.o Application.c	

AMath.o: AMath.c
	$(CC) -o AMath.o AMath.c	

Contract.o: Contract.c
	$(CC) -o Contract.o Contract.c

BinCluster.o: BinCluster.c
	$(CC) -o BinCluster.o BinCluster.c	

UnaryCluster.o: UnaryCluster.c
	$(CC) -o UnaryCluster.o UnaryCluster.c	

FinalCluster.o: FinalCluster.c
	$(CC) -o FinalCluster.o FinalCluster.c	

Data.o: Data.c
	$(CC) -o Data.o Data.c	

Debug.o: Debug.c
	$(CC) -o Debug.o Debug.c	

FreeList.o: FreeList.c
	$(CC) -o FreeList.o FreeList.c

Interface.o: Interface.c
	$(CC) -o Interface.o Interface.c

Nodelist.o: ClusterList.c
	$(CC) -o ClusterList.o ClusterList.c

Queue.o: Queue.c
	$(CC) -o Queue.o Queue.c

PQueue.o: PQueue.c	
	$(CC) -o PQueue.o PQueue.c

Test.o: Test.c
	$(CC) -o Test.o	Test.c

Tree.o: Tree.c
	$(CC) -o Tree.o Tree.c	

Vertex.o: Vertex.c
	$(CC) -o Vertex.o Vertex.c	

Stats.o: Stats.c
	$(CC) -o Stats.o Stats.c

test: $(OBJS)
	$(LINKER) $(LFLAGS) -o test $(OBJS)
clean:
	rm -f *.*~ *.o *.d *.gdb test
