CC = /usr/bin/g++
LD = /usr/bin/g++
RM = /bin/rm

CFLAGS = -c -O3 -g -ggdb -Wall -Wextra -Wno-unused-parameter

LDFLAGS = -l ftdi

BIN = NandTool
SRCS = FtdiNand.cpp  NandChip.cpp  NandData.cpp  NandDataLP.cpp  NandDataSP.cpp  NandID.cpp  NandTool.cpp  stdafx.cpp
OBJS := $(SRCS:.cpp=.o)

all: $(BIN)

.cpp.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ 

$(BIN): $(OBJS)
	$(LD) -o $(BIN) $(OBJS) $(LDFLAGS)

clean:
	$(RM) core $(OBJS) $(BIN)
