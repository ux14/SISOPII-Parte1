all: app server

app:
	g++ ./app/*.cpp -o ./bin/app -Wall -pthread

server:
	g++ ./server/*.cpp -o ./bin/server -Wall -pthread

.PHONY: app server