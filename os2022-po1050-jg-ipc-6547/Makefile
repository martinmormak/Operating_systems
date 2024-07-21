CC = g++
CFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS =
SOURCES = proc_p1.cpp proc_p2.cpp proc_t.cpp proc_d.cpp proc_serv2.cpp zadanie.cpp
HEADERS = 
OBJDIR = obj
EXECUTABLE = proc_p1 proc_p2 proc_t proc_d proc_serv2 zadanie
ERRORFILES = proc_pr.err proc_s.err proc_serv1.err proc_p1.err proc_p2.err proc_t.err proc_d.err proc_serv2.err zadanie.err
OUTFILES = proc_pr.out proc_s.out proc_serv1.out proc_p1.out proc_p2.out proc_t.out proc_d.out proc_serv2.out zadanie.out

all: $(EXECUTABLE)

proc_p1: proc_p1.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

proc_p2: proc_p2.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

proc_t: proc_t.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

proc_d: proc_d.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

proc_serv2: proc_serv2.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

zadanie: zadanie.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJDIR)/*.o $(EXECUTABLE) $(ERRORFILES) $(OUTFILES)
