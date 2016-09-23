LDFLAGS+=-L/usr/local/ssl/lib -lssl -lcrypto -ldl -lm -lsqlite3 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/ssl/lib
client: $(client)
client:client.o cJSON.o
	$(CC) $(CFLAGS) -o client cJSON.o  client-ssl.c $(LDFLAGS)  -I/usr/local/ssl/include 
cJSON.o:cJSON.c cJSON.h
	$(CC) $(CFLAGS) -c -o cJSON.o cJSON.c
client.o:client-ssl.c
	$(CC) $(CFLAGS) -c -o client.o client-ssl.c
clean: 
	rm -f *.o client
