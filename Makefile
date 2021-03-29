all: app server

app:
	g++ -std=c++14 ./app/*.cpp -g -o ./bin/app -Wall -pthread

server:
	g++ -std=c++14 ./server/*.cpp -g -o ./bin/server -Wall -pthread

clean:
	rm -f ./bin/app
	rm -f ./bin/server
	rm -f ./server/users.txt
	rm -f ./server/followers/*.txt

.PHONY: app server clean