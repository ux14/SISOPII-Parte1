all: cliente servidor

cliente:
	g++ ./cliente/*.cpp -o ./bin/cliente -Wall -pthread

servidor:
	g++ ./servidor/*.cpp -o ./bin/servidor -Wall -pthread

.PHONY: cliente
.PHONY: servidor