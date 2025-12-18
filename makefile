PROG = main

SRC = main.cpp
CFLAGS = -g -std=c++17 -Wall
LIBS = -lcurl

CC = g++
OUTPUTDIR = obj
OBJS = $(addprefix $(OUTPUTDIR)/, $(SRC:.cpp=.o))

all: $(OUTPUTDIR) $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(OUTPUTDIR)/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUTDIR):
	mkdir -p $(OUTPUTDIR)

clean:
	rm -rf $(OUTPUTDIR) $(PROG)

.PHONY: clean
