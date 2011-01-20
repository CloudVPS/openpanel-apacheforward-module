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

install:
	mkdir -p ${DESTDIR}/var/openpanel/modules/ApacheForward.module
	mkdir -p ${DESTDIR}/var/openpanel/conf/staging/ApacheForward
	install -m 755 action ${DESTDIR}/var/openpanel/modules/ApacheForward.module/action
	cp     module.xml	  ${DESTDIR}/var/openpanel/modules/ApacheForward.module/module.xml
	install -m 755 verify      ${DESTDIR}/var/openpanel/modules/ApacheForward.module/verify
	cp *.html ${DESTDIR}/var/openpanel/modules/ApacheForward.module


SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/var/openpanel/api/grace/include -I/usr/include/opencore -c $<
