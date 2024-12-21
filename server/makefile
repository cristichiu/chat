BIN = bin
HEADERS = headers
HEADERS_BIN = $(BIN)/$(HEADERS)
HEADERS_ALL = $(HEADERS_BIN)/user.o $(HEADERS_BIN)/lib.o $(HEADERS_BIN)/grup.o $(HEADERS_BIN)/utilities.o $(HEADERS_BIN)/message.o $(HEADERS_BIN)/auth.o $(HEADERS_BIN)/authActions.o $(HEADERS_BIN)/authUtilities.o $(HEADERS_BIN)/grupActions.o
MAINC = main.c
ATTR = -lssl -lcrypto
INCLUDESSH = -I/usr/include/openssl

all: clear $(BIN)/main.exe

# pentru a crea directorii din bin
$(BIN):
	mkdir $(BIN)
$(HEADERS_BIN): $(BIN)
	mkdir $(HEADERS_BIN)

# compilarea headerelor
$(HEADERS_BIN)/user.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/db/code/user.c -o $(HEADERS_BIN)/user.o
$(HEADERS_BIN)/authActions.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/socket/code/authActions.c -o $(HEADERS_BIN)/authActions.o
$(HEADERS_BIN)/auth.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/socket/code/auth.c -o $(HEADERS_BIN)/auth.o
$(HEADERS_BIN)/authUtilities.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/socket/code/authUtilities.c -o $(HEADERS_BIN)/authUtilities.o
$(HEADERS_BIN)/grupActions.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/socket/code/grupActions.c -o $(HEADERS_BIN)/grupActions.o
$(HEADERS_BIN)/grup.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/db/code/grup.c -o $(HEADERS_BIN)/grup.o
$(HEADERS_BIN)/utilities.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/db/code/utilities.c -o $(HEADERS_BIN)/utilities.o
$(HEADERS_BIN)/message.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/db/code/message.c -o $(HEADERS_BIN)/message.o
$(HEADERS_BIN)/lib.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/lib.c -o $(HEADERS_BIN)/lib.o

# compilarea main.c
$(BIN)/main.exe: $(HEADERS_ALL)
	gcc $(MAINC) $(HEADERS_ALL) -o $(BIN)/main.exe $(ATTR)

# clear
clear:
	rm $(HEADERS_BIN) -rf
	rm $(BIN)/main.exe
create: $(BIN)/main.exe
