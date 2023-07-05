CC = /usr/bin/g++

CFLAGS = -std=c++14 -fcolor-diagnostics -fansi-escape-codes -g

LDFLAGS = -ljsoncpp			\
		-lmicrohttpd -ljsonrpccpp-common		\
		-ljsonrpccpp-server -ljsonrpccpp-client -lcurl

INC	= general.h

hw5client:	gps.o time.o person.o thing.o hw5client.o
	$(CC) gps.o time.o person.o thing.o hw5client.o -o hw5client -g $(LDFLAGS)

gps.o:  gps.cpp gps.h $(INC)
	$(CC) -c $(CFLAGS) gps.cpp

time.o: time.cpp $(INC)
	$(CC) -c $(CFLAGS) time.cpp

person.o: person.cpp person.h $(INC)
	$(CC) -c $(CFLAGS) person.cpp

thing.o: thing.cpp thing.h $(INC)
	$(CC) -c $(CFLAGS) thing.cpp

hw5client.o: hw5client.cpp $(INC)
	$(CC) -c $(CFLAGS) hw5client.cpp

clean:
	rm *.o
	rm hw5client
