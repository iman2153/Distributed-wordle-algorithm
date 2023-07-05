
# Makefile for f2022, ecs36b
# https://macappstore.org/libjson-rpc-cpp/
# https://github.com/cinemast/libjson-rpc-cpp

CC = /usr/bin/g++ -std=c++17
CFLAGS = -g

LDFLAGS = 	-L/opt/homebrew/lib/ -ljsoncpp			\
		-lmicrohttpd -ljsonrpccpp-common		\
		-ljsonrpccpp-server -ljsonrpccpp-client -lcurl

INC	=	Common.h

INC_CL	=	JvTime.h
OBJ	=	JvTime.o

# rules.
all: 	w_server w_client

#
#
# <target> : [... tab...] <dependency>
# [... tab ....] <action>

w_client.h:		w_.json
	jsonrpcstub w_.json --cpp-server=w_Server --cpp-client=w_Client

w_server.h:		ecs36b_hw6.json
	jsonrpcstub w_.json --cpp-server=w_Server --cpp-client=w_Client

w_client.o:		w_client.cpp w_client.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) w_client.cpp

w_server.o:		w_server.cpp w_server.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) w_server.cpp

w_JSON.o:		w_JSON.cpp $(INC)
	$(CC) -c $(CFLAGS) w_JSON.cpp

JvTime.o:	JvTime.cpp JvTime.h $(INC)
	$(CC) -c $(CFLAGS) JvTime.cpp

w_server:	w_server.o ecs36b_JSON.o $(OBJ)
	$(CC) -o w_server w_server.o w_JSON.o $(OBJ) $(LDFLAGS)

w_client:	w_client.o w_JSON.o $(OBJ)
	$(CC) -o w_client w_client.o w_JSON.o $(OBJ) $(LDFLAGS)

clean:
	rm -f *.o *~ core w_client.h w_server.h w_client w_server


