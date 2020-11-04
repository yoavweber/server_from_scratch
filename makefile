CC = g++

output: server.o socket.o
	g++ server.o socket.o -o output

main.o: server.cc
	g++ -c server.cc

socket.o: net/socket.cpp net/socket.h
	g++ -c net/socket.cpp

clean:
	rm *.o output