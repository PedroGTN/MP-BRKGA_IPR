

CC = g++
CFLAGS = -Wall -pedantic -O3 -Wno-unused-result
OBJS = Tspdproblem.o Tspdsolver.o Digrafo.o Digrafo2.o
MAIN = tspd.o
BIN = tspd


all: $(OBJS) $(MAIN) h-fstsp tsp
	$(CC) -o $(BIN) $(CFLAGS) $(OBJS) $(MAIN)

h-fstsp: $(OBJS) h-fstsp.cpp
	$(CC) -o h-fstsp $(CFLAGS) $(OBJS) h-fstsp.cpp
	
tsp: $(OBJS) tsp.cpp
	$(CC) -o tsp $(CFLAGS) $(OBJS) tsp.cpp


clean:
	rm *.o $(BIN) h-fstsp tsp


## g++ -Wall -pedantic -O2 -Wno-unused-result Tspdproblem.cpp Tspdsolver.cpp Digrafo.cpp Digrafo2.cpp h-fstsp.cpp -o h-fstsp

##tspd.cpp 
## - tem a main()
## - importa "Tspdproblem.h"
## - importa "Tspdsolver.h"
## ==> g++ -Wall -pedantic -O2 -Wno-unused-result Tspdproblem.o Tspdsolver.o Digrafo.o Digrafo2.o tspd.cpp -o tspd
##    - gera "Tspdproblem.o"
##
##
##Tspdproblem.cpp 
## - implementa "Tspdproblem.h"
## - importa "configs.h"
## ==> g++ -Wall -pedantic -O2 -Wno-unused-result -c Tspdproblem.cpp
##    - gera "Tspdproblem.o"
##	
##Tspdsolver.cpp
## - implementa "Tspdsolver.h"
## - importa "Digrafo.h"
## - importa "Digrafo2.h"
## - importa "configs.h"
## ==> g++ -Wall -pedantic -O2 -Wno-unused-result -c Tspdsolver.cpp
##    - gera "Tspdsolver.o"
##
##Digrafo.cpp
## - implementa "Digrafo.h"
## - importa "configs.h"
## ==> g++ -Wall -pedantic -O2 -Wno-unused-result -c Digrafo.cpp
##    - gera "Digrafo.o"
##
##Digrafo2.cpp
## - implementa "Digrafo2.h"
## - importa "configs.h"
## ==> g++ -Wall -pedantic -O2 -Wno-unused-result -c Digrafo2.cpp
##    - gera "Digrafo2.o"
