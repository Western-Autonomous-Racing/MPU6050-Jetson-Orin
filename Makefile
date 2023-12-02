CC=g++

DEPS=MPU6050.h
CFLAGS=-fPIC -Wall

LIBS=-li2c
LFLAGS=-shared

OBJ=MPU6050.o
OLIB=libMPU6050.so
LIB_DIR=lib

$(LIB_DIR)/%.o: %.cpp $(DEPS)
	mkdir -p $(LIB_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

all: $(LIB_DIR)/$(OBJ)
	$(CC) -o $(LIB_DIR)/$(OLIB) $< $(LIBS) $(LFLAGS)

install: all $(LIB_DIR)/$(OLIB) $(DEPS)
	sudo install -m 755 -p $(LIB_DIR)/$(OLIB) /usr/lib/
	sudo install -m 644 -p $(DEPS) /usr/include/

clean:
	rm -rf $(LIB_DIR)

uninstall: clean
	sudo rm -rf /usr/lib/$(OLIB)
	sudo rm -rf /usr/include/$(DEPS)

example: $(LIB_DIR)/Example.o
	$(CC) $< -o $(LIB_DIR)/Example -lMPU6050 -pthread