BIN = bin
HEADERS = headers
HEADERS_BIN = $(BIN)/$(HEADERS)
HEADERS_ALL = $(HEADERS_BIN)/auth.o $(HEADERS_BIN)/lib.o $(HEADERS_BIN)/authActions.o $(HEADERS_BIN)/grupActions.o
MAINC = client.c
ATTR = -lssl -lcrypto

all: clear $(BIN)/main.exe

# pentru a crea directorii din bin
$(BIN):
	mkdir $(BIN)
$(HEADERS_BIN): $(BIN)
	mkdir $(HEADERS_BIN)

# compilarea headerelor
$(HEADERS_BIN)/auth.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/action/code/auth.c -o $(HEADERS_BIN)/auth.o
$(HEADERS_BIN)/grupActions.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/action/code/grupActions.c -o $(HEADERS_BIN)/grupActions.o
$(HEADERS_BIN)/authActions.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/action/code/authActions.c -o $(HEADERS_BIN)/authActions.o
$(HEADERS_BIN)/lib.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/action/code/lib.c -o $(HEADERS_BIN)/lib.o

# compilarea main.c
$(BIN)/main.exe: $(HEADERS_ALL)
	gcc $(MAINC) $(HEADERS_ALL) -o $(BIN)/main.exe $(ATTR)

# clear
clear:
	rm $(HEADERS_BIN) -rf
	rm $(BIN)/main.exe
create: $(BIN)/main.exe
