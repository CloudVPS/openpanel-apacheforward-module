include makeinclude

OBJ	= main.o

all: module.xml action

action: $(OBJ)
	$(LD) $(LDFLAGS) -o action $(OBJ) -L/var/opencore/api/grace/lib -L../opencore/api/grace/lib -lgrace-coreapi $(LIBS)

module.xml: module.def
	mkmodulexml < module.def > module.xml

clean:
	rm -f *.o action module.xml

makeinclude:
	@echo please run ./configure
	@false

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/var/opencore/api/grace/include -I../opencore/api/grace/include -c $<
