SOURCES = $(wildcard *.cxx)

OBJECTS = $(SOURCES:%.cxx=%.o)

all: install 

install: $(LIBRARY)
	cp -f $(LIBRARY) ../lib

$(OBJECTS): $(SOURCES)
	$(COMP) -g -c  $(DEFINES) $(INCLUDES) $?

$(LIBRARY): $(OBJECTS)
	ar -rv $(LIBRARY) $? 
	ranlib $(LIBRARY)

clean:
	rm *.o
