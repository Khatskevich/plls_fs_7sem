PROGPATH=.
INCLUDES=$(PROGPATH)/include
SRC_PATH=$(PROGPATH)/src
SRC_FILES=$(SRC_PATH)/fat_reader.c $(SRC_PATH)/fs_driver.c $(SRC_PATH)/log.c
all:  $(SRC_FILES)
	$(CC) $(SRC_FILES) -I$(INCLUDES) -pthread
