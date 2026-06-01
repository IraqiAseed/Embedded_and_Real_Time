CC = gcc

BUILD_DIR = build
BIN_DIR = bin

CLIENT_SRC_DIR = client/src
CLIENT_INC_DIR = client/include

SERVER_SRC_DIR = server/src
SERVER_INC_DIR = server/include

COMMON_SRC_DIR = common/src
COMMON_INC_DIR = common/include

MANAGERS_SRC_DIR = managers/src
MANAGERS_INC_DIR = managers/include

DS_INC_DIR = external/ds/include
DS_LIB_DIR = external/ds/lib

CFLAGS = -Wall -Wextra -g \
         -I$(COMMON_INC_DIR) \
         -I$(CLIENT_INC_DIR) \
         -I$(SERVER_INC_DIR) \
         -I$(MANAGERS_INC_DIR) \
         -I$(DS_INC_DIR)

LDFLAGS = -L$(DS_LIB_DIR) -Wl,-rpath,'$$ORIGIN/../external/ds/lib' -lds

CLIENT_BIN = $(BIN_DIR)/client.out
SERVER_BIN = $(BIN_DIR)/server.out

CLIENT_OBJ = $(BUILD_DIR)/client.o
CLIENT_NETWORKING_OBJ = $(BUILD_DIR)/client_networking.o
CLIENT_MANAGER_OBJ = $(BUILD_DIR)/client_manager.o
CLIENT_CHAT_OBJ = $(BUILD_DIR)/client_chat.o

SERVER_OBJ = $(BUILD_DIR)/server.o
SERVER_NETWORKING_OBJ = $(BUILD_DIR)/server_networking.o
SERVER_MANAGER_OBJ = $(BUILD_DIR)/server_manager.o

PROTOCOL_OBJ = $(BUILD_DIR)/protocol.o
MULTICAST_OBJ = $(BUILD_DIR)/multicast.o
CHAT_IPC_OBJ = $(BUILD_DIR)/chat_ipc.o

USER_MANAGER_OBJ = $(BUILD_DIR)/user_manager.o
GROUP_MANAGER_OBJ = $(BUILD_DIR)/group_manager.o

.PHONY: all clean run-client run-server

all: $(CLIENT_BIN) $(SERVER_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(CLIENT_OBJ): $(CLIENT_SRC_DIR)/client.c $(CLIENT_INC_DIR)/client.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_NETWORKING_OBJ): $(CLIENT_SRC_DIR)/client_networking.c $(CLIENT_INC_DIR)/client_networking.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_MANAGER_OBJ): $(CLIENT_SRC_DIR)/client_manager.c $(CLIENT_INC_DIR)/client_manager.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_CHAT_OBJ): $(CLIENT_SRC_DIR)/client_chat.c $(CLIENT_INC_DIR)/client_chat.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_NETWORKING_OBJ): $(SERVER_SRC_DIR)/server_networking.c $(SERVER_INC_DIR)/server_networking.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(SERVER_MANAGER_OBJ): $(SERVER_SRC_DIR)/server_manager.c $(SERVER_INC_DIR)/server_manager.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_OBJ): $(SERVER_SRC_DIR)/server.c \
               $(SERVER_INC_DIR)/server.h \
               $(COMMON_INC_DIR)/protocol.h \
               $(MANAGERS_INC_DIR)/user_manager.h \
               $(MANAGERS_INC_DIR)/group_manager.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(PROTOCOL_OBJ): $(COMMON_SRC_DIR)/protocol.c $(COMMON_INC_DIR)/protocol.h $(COMMON_INC_DIR)/limitation.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MULTICAST_OBJ): $(COMMON_SRC_DIR)/multicast.c $(COMMON_INC_DIR)/multicast.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CHAT_IPC_OBJ): $(COMMON_SRC_DIR)/chat_ipc.c $(COMMON_INC_DIR)/chat_ipc.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(USER_MANAGER_OBJ): $(MANAGERS_SRC_DIR)/user_manager.c $(MANAGERS_INC_DIR)/user_manager.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(GROUP_MANAGER_OBJ): $(MANAGERS_SRC_DIR)/group_manager.c $(MANAGERS_INC_DIR)/group_manager.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_BIN): $(CLIENT_OBJ) \
               $(CLIENT_NETWORKING_OBJ) \
               $(CLIENT_MANAGER_OBJ) \
               $(CLIENT_CHAT_OBJ) \
               $(PROTOCOL_OBJ) \
               $(MULTICAST_OBJ) \
               $(CHAT_IPC_OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(SERVER_BIN): $(SERVER_OBJ) \
               $(SERVER_NETWORKING_OBJ) \
               $(SERVER_MANAGER_OBJ) \
               $(PROTOCOL_OBJ) \
               $(USER_MANAGER_OBJ) \
               $(GROUP_MANAGER_OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

run-server: $(SERVER_BIN)
	./$(SERVER_BIN)

run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)