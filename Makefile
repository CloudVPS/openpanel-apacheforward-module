# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/
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
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/var/openpanel/api/grace/include -I/usr/include/opencore -c $<
