CC = gcc

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
CLIENT_SRC_DIR = client/src
CLIENT_INC_DIR = client/include

DS_INC_DIR = external/ds/include
DS_LIB_DIR = external/ds/lib

CFLAGS = -Wall -Wextra -g -I$(INC_DIR) -I$(CLIENT_INC_DIR) -I$(DS_INC_DIR)
LDFLAGS = -L$(DS_LIB_DIR) -Wl,-rpath,'$$ORIGIN/../external/ds/lib' -lds

CLIENT_BIN = $(BIN_DIR)/client.out
SERVER_BIN = $(BIN_DIR)/server.out

PROTOCOL_OBJ = $(BUILD_DIR)/protocol.o
CLIENT_OBJ = $(BUILD_DIR)/client.o
CLIENT_CHAT_OBJ = $(BUILD_DIR)/client_chat.o
SERVER_OBJ = $(BUILD_DIR)/server.o
USER_MANAGER_OBJ = $(BUILD_DIR)/user_manager.o
GROUP_MANAGER_OBJ = $(BUILD_DIR)/group_manager.o
MULTICAST_OBJ = $(BUILD_DIR)/multicast.o
CHAT_IPC_OBJ = $(BUILD_DIR)/chat_ipc.o


.PHONY: all clean run-client run-server

all: $(CLIENT_BIN) $(SERVER_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(PROTOCOL_OBJ): $(SRC_DIR)/protocol.c $(INC_DIR)/protocol.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(USER_MANAGER_OBJ): $(SRC_DIR)/user_manager.c $(INC_DIR)/user_manager.h $(INC_DIR)/protocol.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ): $(CLIENT_SRC_DIR)/client.c $(CLIENT_INC_DIR)/client.h $(INC_DIR)/protocol.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(CLIENT_CHAT_OBJ): client/src/client_chat.c client/include/client_chat.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_OBJ): $(SRC_DIR)/server.c $(INC_DIR)/protocol.h $(INC_DIR)/user_manager.h $(INC_DIR)/group_manager.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(GROUP_MANAGER_OBJ): $(SRC_DIR)/group_manager.c $(INC_DIR)/group_manager.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(MULTICAST_OBJ): $(SRC_DIR)/multicast.c $(INC_DIR)/multicast.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(CHAT_IPC_OBJ): $(SRC_DIR)/chat_ipc.c $(INC_DIR)/chat_ipc.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_BIN): $(CLIENT_OBJ) $(CLIENT_CHAT_OBJ) $(PROTOCOL_OBJ) $(MULTICAST_OBJ) $(CHAT_IPC_OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(SERVER_BIN): $(SERVER_OBJ) $(PROTOCOL_OBJ) $(USER_MANAGER_OBJ) $(GROUP_MANAGER_OBJ)| $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

run-server: $(SERVER_BIN)
	./$(SERVER_BIN)

run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)