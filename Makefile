all:
	c++ src/main.cpp -o flocking_simulator -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && \
	c++ src/main_real_time.cpp -o real_time_flocking_simulator -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f flocking_simulator && rm -f real_time_flocking_simulator