######################################################################
## Makefile
##
## Umut A. Acar
######################################################################

CC = g++
OPTFLAGS = -O4 -finline-functions 
COPTIONS = -Wno-deprecated -fpermissive
RCFLAGS = -DNDEBUG -DNVERBOSE 
LDOPTIONS = 
INCLUDES = -I. -I../rcTrees
CFLAGS = $(COPTIONS) $(OPTFLAGS) $(INCLUDES)
LIBS = -L.
LD = $(CC) $(LIBS) $(LDOPTIONS) 

APOBJS = Data.o Application.o

default:  Example
.c.o: 
	$(CC) $(CFLAGS) $(RCFLAGS) -c $*.c

libRCTrees: $(APOBJS) 
	make rcobjs -f ../rcTrees/Makefile -C.   # generates libRCTrees.a
	ar r libRCTrees.a $(APOBJS)      # insert the application files

Example : Example.o libRCTrees
	$(LD) -o Example Example.o -lRCTrees

clean:
	rm -f *.o *.a Example
