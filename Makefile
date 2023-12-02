CC=g++

DEPS=MPU6050.h
CFLAGS=-fPIC -Wall

LIBS=-li2c
LFLAGS=-shared

OBJ=MPU6050.o
OLIB=libMPU6050.so
BUILD_DIR=build
LIB_DIR=lib

$(BUILD_DIR)/%.o: %.cpp $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

all: $(BUILD_DIR)/$(OBJ)
	$(CC) -o $(BUILD_DIR)/Example $(BUILD_DIR)/Example.o -lMPU6050 -pthread

$(BUILD_DIR)/$(OBJ): $(OBJ)
	mkdir -p $(BUILD_DIR)
	mv $< $(BUILD_DIR)

$(LIB_DIR)/$(OLIB): $(OBJ)
	mkdir -p $(LIB_DIR)
	$(CC) -o $(LIB_DIR)/$(OLIB) $< $(LIBS) $(LFLAGS)

install: all $(LIB_DIR)/$(OLIB) $(DEPS)
	install -m 755 -p $(LIB_DIR)/$(OLIB) /usr/lib/
	install -m 644 -p $(DEPS) /usr/include/

uninstall:
	rm -f /usr/include/MPU6050.h
	rm -f /usr/lib/$(OLIB)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(LIB_DIR)
	$(CC) -o $(OLIB) $< $(LIBS) $(LFLAGS)

install: all $(OLIB) $(DEPS)
	install -m 755 -p $(OLIB) /usr/lib/
	install -m 644 -p $(DEPS) /usr/include/

uninstall:
	rm -f /usr/include/MPU6050.h
	rm -f /usr/lib/libMPU6050.so

clean:
	rm -f Example
	rm -f Example.o
	rm -f MPU6050.o
	rm -f libMPU6050.so

example: Example.o
	$(CC) $< -o Example -lMPU6050 -pthread
