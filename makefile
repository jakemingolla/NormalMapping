EXE = lab9-linux-go
CFLAGS = -g
DLIBS = -L./ -L/comp/175/lib/ -lGL -lGLU -lglut -lglui
SLIBS = /comp/175/lib/libGLEW.a
INC = -I./ -I/comp/175/include
OBJ = ppm.o shaderManager.o main.o

$(EXE):$(OBJ)
	g++ $(CFLAGS) $(INC) -o $@ $(OBJ) $(DLIBS) $(SLIBS)

%.o : %.cpp
	g++ $(CFLAGS) $(INC) -c $<

clean:
	rm -rf *.o *~ $(EXE)
