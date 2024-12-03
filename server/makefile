BIN = bin
HEADERS = headers
HEADERS_BIN = $(BIN)/$(HEADERS)
HEADERS_ALL = $(HEADERS_BIN)/db.o $(HEADERS_BIN)/lib.o
#HEADERS_ALL = $(HEADERS_BIN)/lib.o

all: clear $(BIN)/main.exe

# pentru a crea directorii din bin
$(BIN):
	mkdir $(BIN)
$(HEADERS_BIN): $(BIN)
	mkdir $(HEADERS_BIN)

# compilarea headerelor
$(HEADERS_BIN)/db.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/db.c -o $(HEADERS_BIN)/db.o
$(HEADERS_BIN)/lib.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/lib.c -o $(HEADERS_BIN)/lib.o

# compilarea main.c
$(BIN)/main.exe: $(HEADERS_ALL)
	gcc main.c $(HEADERS_ALL) -o $(BIN)/main.exe

# clear
clear:
	rm $(HEADERS_BIN) -rf
	rm $(BIN)/main.exe
create: $(BIN)/main.exe