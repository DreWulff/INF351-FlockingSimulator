all:
	c++ src/main.cpp -o flocking_simulator -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f flocking_simulator