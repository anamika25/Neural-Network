all: Backprop.exe

clean:
    rm Backprop.o Backprop.exe

Backprop.exe: Backprop.o
    g++ -g -o Backprop Backprop.o

Backprop.o:
    g++ -c -g Backprop.cpp