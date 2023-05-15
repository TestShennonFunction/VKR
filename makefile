CC=g++
CF=-Wall -fopenmp -O3
TARGET=htst
OBJECTS=settings.o tcommon.o combinations.o init.o diagtest.o anytest.o alltest.o utils.o main.o
all: htst fast
htst: $(OBJECTS)
	$(CC) $(CF) -o $(TARGET) $(OBJECTS)
fast: 
	$(CC) $(CF) -o $(TARGET) $(OBJECTS) 
.cpp.o:
	$(CC) -c $(CF) $<
main.o: settings.h tcommon.h combinations.h init.h diagtest.h anytest.h alltest.h utils.h main.cpp
settings.o: settings.h settings.cpp
tcommon.o: settings.h tcommon.h tcommon.cpp
init.o: init.h init.cpp settings.h tcommon.h
combinations.o: combinations.h combinations.cpp settings.h tcommon.h
diagtest.o: diagtest.cpp diagtest.h init.h settings.h tcommon.h combinations.h 
anytest.o: anytest.cpp anytest.h init.h settings.h tcommon.h combinations.h 
alltest.o: alltest.cpp alltest.h init.h settings.h tcommon.h combinations.h 
utils.o: utils.cpp utils.h alltest.h init.h settings.h tcommon.h combinations.h 
clean: 
	rm *.o $(TARGET)
