CC=clang++
	
message_printer : main.o message.o
	$(CC)  main.o message.o -o message_printer


main.o : main.cpp message.cpp # message.h
	$(CC) -c main.cpp -o main.o
    
message.o: message.cpp # message.h
	$(CC) -c message.cpp -o message.o

.PHONY : purge clean

clean:
	rm *.o
	
purge:
	- rm *.o
	- rm message_printer
