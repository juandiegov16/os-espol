CC = gcc
CFLAGS = -Wall -c -I$(INCLUDE) $^ -o $@
OFLAGS = -Wall -I$(INCLUDE) $^ -o $@ -lpthread

INCLUDE = .
SRC_DIR = .
OBJ_DIR = .

CSAPP = $(OBJ_DIR)/csapp.o
DIJKSTRA = $(OBJ_DIR)/dijkstra.o

SERVER = Graph_Server
CLIENT = Result_Client 

I_FILES = $(wildcard $(INCLUDE)/*.h)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXE_FILES =  $(SERVER) $(CLIENT)

.PHONY: all debug sanitize clean dir help server client
all: $(OBJ_FILES) $(SERVER) $(CLIENT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS)

$(SERVER): $(OBJ_DIR)/Graph_Server.o $(CSAPP)
	$(CC) $(OFLAGS) $(DFLAGS)

$(CLIENT): $(OBJ_DIR)/Result_Client.o $(CSAPP)
	$(CC) $(OFLAGS) $(DFLAGS)


server: $(SERVER)
	./Graph_Server
client: $(CLIENT)
	./Result_Client

## debug: Compila usando la opción -g para facilitar la depuración con gdb.
debug: DFLAGS = -g
debug: clean all

## sanitize: Compila habilitando la herramienta AddressSanitizer para facilitar la depuración en tiempo de ejecución.
sanitize: DFLAGS = -fsanitize=address,undefined
sanitize: clean all

## clean: Limpia archivos autogenerados
clean:
	rm -rf $(EXE_FILES) $(OBJ_FILES)

## dir: Muestra contenido del proyecto en formato largo
dir:
	ls -lhR

help : Makefile
	@sed -n 's/^##//p' $<	