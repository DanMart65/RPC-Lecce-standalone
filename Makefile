

ARCH := $(shell basename `uname -s`)

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTINC      := $(shell root-config --incdir)

ifeq ($(ARCH),Darwin)
# for MacOSX
LIBS = -lcontrol -ldetector -ldatabase -lcontrol -levent -ldatabase -ltools -luser -lrecon -ldetector -lhis
LKOPT = -Xlinker -bind_at_load -flat_namespace
DEFINES = -DMOTOROLA
# for Mac OS X 10.2 gcc2.96
# CXX = g++2 -O -pipe -fPIC -Wall -D_REENTRANT
# for Mac Os 10.1 gcc3.1
CXX = g++ -w -fPIC -g  `root-config --cflags`
endif

ifeq ($(ARCH),Linux)
# for Linux AFS
#LIBS = -lcontrol -ldetector -ldatabase -lcontrol -levent -ldatabase -ltools -luser -lrecon -ldetector -lhis -lCLHEP -lmathlib -lpacklib -lkernlib -lnsl -lg2c
LIBS = -lcontrol -ldetector -ldatabase -lcontrol -levent -ldatabase -ltools -luser -lrecon -ldetector -lhis 
LKOPT = -O
DEFINES =
CXX = g++ -O -w -fPIC -g  `root-config --cflags`
endif


CERNDIR = /PATH-TO-ROOT/root/installation_dir
INCLUDES = -I./ -I../ -I$(CERNDIR)/include  
MYLIBS = -L./control/ -L./detector/ -L./database/ -L./event/ -L./tools/ -L./user/ -L./recon/ -L./his
LIBDIR = -L$(CERNDIR)/lib $(MYLIBS)
 
SUBLIBS = libcontrol.a libdetector.a libdatabase.a libevent.a libtools.a librecon.a libuser.a libhis.a 
SUBDIRS = control event tools detector recon user his
SOURCES = $(wildcard *.cxx)
OBJECTS = $(SOURCES:%.cxx=%.o)

all: calib

calib:  $(SUBLIBS) $(OBJECTS)
	$(CXX) $(LKOPT) -o $(HOME)/bin/lets calib.o $(ROOTGLIBS) $(LIBDIR) $(LIBS) $(ROOTGLIBS)

libcontrol.a: control
	cd $?; gmake COMP="$(CXX)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

libdatabase.a: database
	cd $?; gmake COMP="$(CXX)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

libevent.a: event
	cd $?; gmake COMP="$(CXX)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

libtools.a: tools
	cd $?; gmake COMP="$(CXX)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

libdetector.a: detector
	cd $?; gmake COMP="$(CXX)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

libuser.a: user
	cd $?; gmake COMP="$(CXX)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

librecon.a: recon
	cd $?; gmake COMP="$(CXX)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

libhis.a: his
	cd $?; gmake COMP="$(CXX)" CERNDIR="$(CERNDIR)" DEFINES="$(DEFINES)" INCLUDES="$(INCLUDES)" LIBRARY="$@" all

$(OBJECTS): $(SOURCES)
	$(CXX) -g -c  $(DEFINES) $(INCLUDES) $?

clean:
	rm *.o */*.o */*.a
