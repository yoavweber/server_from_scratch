CC = g++

output: server.o socket.o routes.o responses.o parser.o webSocket.o
	g++ server.o socket.o routes.o responses.o parser.o webSocket.o -lssl -lcrypto -o output

main.o: server.cc
	g++ -c server.cc

socket.o: net/socket.cpp net/socket.h
	g++ -c net/socket.cpp

routes.o: http/routes.cpp http/routes.h
	g++ -c http/routes.cpp

responses.o: http/responses.cpp http/responses.h
	g++ -c http/responses.cpp

parser.o: http/parser.cpp http/parser.h
	g++ -c http/parser.cpp

webSocket.o: net/webSocket.cpp net/webSocket.h
	g++ -c net/webSocket.cpp 


clean:
	rm *.o output