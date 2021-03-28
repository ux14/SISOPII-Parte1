all: app server

app:
	g++ -std=c++11 ./app/*.cpp -g -o ./bin/app -Wall -pthread

server:
	g++ -std=c++11 ./server/*.cpp -g -o ./bin/server -Wall -pthread

.PHONY: app server