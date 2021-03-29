all: app server

app:
	g++ -std=c++11 ./app/*.cpp -g -o ./bin/app -Wall -pthread

server:
	g++ -std=c++11 ./server/*.cpp -g -o ./bin/server -Wall -pthread

clean:
	rm -f ./bin/app
	rm -f ./bin/server

clean-files:
	rm -f ./server/files/users.txt
	rm -f ./server/files/followers/*.txt

.PHONY: app server clean clean-files