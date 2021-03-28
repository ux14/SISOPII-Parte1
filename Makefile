all: app server

app:
	g++ -std=c++11 ./app/*.cpp -o ./bin/app -Wall -pthread

server:
	g++ -std=c++11 ./server/*.cpp -o ./bin/server -Wall -pthread

.PHONY: app server