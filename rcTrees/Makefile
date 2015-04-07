######################################################################
## Makefile
##
## Umut A. Acar
######################################################################

CC = g++
OPTFLAGS = -O4 -finline-functions 
COPTIONS = -Wno-deprecated -Wno-unused-result
RCFLAGS = -DNDEBUG -DNVERBOSE 
LDOPTIONS = 
INCLUDES = -I. -I../rcTrees #-g
CFLAGS = $(COPTIONS) $(OPTFLAGS) $(INCLUDES)
LIBS = -L.
LD = $(CC) $(LIBS) $(LDOPTIONS) 

RCOBJS = AMath.o Debug.o FreeList.o Contract.o BinCluster.o UnaryCluster.o FinalCluster.o Interface.o  Queue.o Tree.o Vertex.o ClusterList.o 

%.o: 
	$(CC) $(CFLAGS) $(RCFLAGS) -c ../rcTrees/$(subst .o,.c,$@)

rcobjs : $(RCOBJS)
	ar rc libRCTrees.a $(RCOBJS) 

clean:
	rm -f *.o libRCTrees.a
