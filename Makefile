include makeinclude

OBJ	= main.o

all: module.xml action

action: $(OBJ)
	$(LD) $(LDFLAGS) -o action $(OBJ) -L/usr/lib/opencore -lgrace-coreapi $(LIBS)

module.xml: module.def
	mkmodulexml < module.def > module.xml

clean:
	rm -f *.o action module.xml

makeinclude:
	@echo please run ./configure
	@false

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/usr/include/opencore -c $<
