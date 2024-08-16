BIN_DIR := ./bin
INC_DIR := ./include
OBJ_DIR := ./obj
SRC_DIR := ./src

CC := g++
CFLAGS := -Wall -std=c++11 -c -I$(INC_DIR)
TARGET := $(BIN_DIR)/tp3.out

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run:
	@$(TARGET)

clean:
	@rm -r $(BIN_DIR)/* $(OBJ_DIR)/*