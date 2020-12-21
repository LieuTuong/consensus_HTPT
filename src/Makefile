
CC = g++
LFLAGS = -pthread -fno-stack-protector
OBJS = main.o threads.o consensus.o log.o
INCDIR = ./include/
OBJDIR = ./.obj/
LOGDIR = ../logs/
EXEFILE = run
MKDIR_P = mkdir -p 
.PHONY: all mkdirobj



all: mkdirobj $(OBJS)
	$(CC) $(LFLAGS) $(OBJDIR)main.o  \
	$(OBJDIR)config.o $(OBJDIR)threads.o \
	$(OBJDIR)consensus.o $(OBJDIR)log.o \
	-o $(EXEFILE)

mkdirobj:
	$(MKDIR_P) $(OBJDIR) $(LOGDIR)

main.o: main.cpp $(INCDIR)define.h  config.o threads.o consensus.o
	$(CC) $(LFLAGS) -c main.cpp -o $(OBJDIR)main.o


config.o: config.cpp $(INCDIR)define.h $(INCDIR)config.h
	$(CC) -c config.cpp -o $(OBJDIR)config.o

threads.o: threads.cpp $(INCDIR)define.h $(INCDIR)threads.h
	$(CC) -c threads.cpp -o $(OBJDIR)threads.o

consensus.o: consensus.cpp $(INCDIR)define.h $(INCDIR)consensus.h config.o
	$(CC) -c consensus.cpp -o $(OBJDIR)consensus.o

log.o: log.cpp $(INCDIR)define.h $(INCDIR)log.h
	$(CC) -c log.cpp -o $(OBJDIR)log.o
	
clean:
	rm -rf $(OBJDIR) $(EXEFILE)
