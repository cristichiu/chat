BIN = bin
HEADERS = headers
HEADERS_BIN = $(BIN)/$(HEADERS)
HEADERS_ALL = $(HEADERS_BIN)/client_utils.o $(HEADERS_BIN)/auth.o 
MAINC = client.c

all: clear $(BIN)/client.exe

# pentru a crea directorii din bin
$(BIN):
	mkdir $(BIN)
$(HEADERS_BIN): $(BIN)
	mkdir $(HEADERS_BIN)

# compilarea headerelor

$(HEADERS_BIN)/client_utils.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/client_utils.c -o $(HEADERS_BIN)/client_utils.o
$(HEADERS_BIN)/auth.o: $(HEADERS_BIN)
	gcc -c $(HEADERS)/auth.c -o $(HEADERS_BIN)/auth.o
# compilarea main.c
$(BIN)/client.exe: $(HEADERS_ALL)
	gcc $(MAINC) $(HEADERS_ALL) -o $(BIN)/client.exe

# clear
clear:
	rm $(HEADERS_BIN) -rf
	rm $(BIN)/client.exe
create: $(BIN)/client.exe