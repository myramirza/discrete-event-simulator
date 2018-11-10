eventSim: eventSim.c
	gcc -Wall eventSim.c -o eventSim

clean:
	rm lltest $(objects)
