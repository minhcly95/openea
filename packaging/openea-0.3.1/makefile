#!/usr/bin/make
INSTALL = install
INSTALL_FILE    = $(INSTALL) -p    -o root -g root  -m  644
INSTALL_PROGRAM = $(INSTALL) -p    -o root -g root  -m  755
INSTALL_SCRIPT  = $(INSTALL) -p    -o root -g root  -m  755

all:
	@cd EA && $(MAKE) all
	@cd EA-CLI && $(MAKE) all

clean:
	@cd EA && $(MAKE) clean
	@cd EA-CLI && $(MAKE) clean

prefix = /usr/local
bindir = $(prefix)/bin
libdir = $(prefix)/lib
incdir = $(prefix)/include
sharedir = $(prefix)/share
docdir = $(sharedir)/doc

install: install-lib install-inc install-cli

install-lib: all
	@-mkdir -p $(DESTDIR)$(docdir)/libopenea0
	$(INSTALL_FILE) copyright $(DESTDIR)$(docdir)/libopenea0
	$(INSTALL_PROGRAM) EA/libopenea.so.0 $(DESTDIR)$(libdir)/

install-inc: all
	@-mkdir -p $(DESTDIR)$(docdir)/libopenea-dev
	$(INSTALL_FILE) copyright $(DESTDIR)$(docdir)/libopenea-dev
	cp -r EA/include/openea $(DESTDIR)$(incdir)/

install-cli: all
	@-mkdir -p $(DESTDIR)$(docdir)/libopenea-cli
	$(INSTALL_FILE) copyright $(DESTDIR)$(docdir)/libopenea-cli
	$(INSTALL_SCRIPT) EA-CLI/openea $(DESTDIR)$(bindir)/
	$(INSTALL_PROGRAM) EA-CLI/openea-run $(DESTDIR)$(bindir)/
	$(INSTALL_PROGRAM) EA-CLI/openea-addon-update $(DESTDIR)$(bindir)/
	@-mkdir -p $(DESTDIR)$(sharedir)/libopenea-cli
	$(INSTALL_FILE) EA-CLI/Add-on.cpp $(DESTDIR)$(sharedir)/libopenea-cli/
	$(INSTALL_SCRIPT) EA-CLI/addon-make $(DESTDIR)$(sharedir)/libopenea-cli/
	$(INSTALL_SCRIPT) EA-CLI/remotelaunch.sh $(DESTDIR)$(sharedir)/libopenea-cli/
	cp -r EA-CLI/sample $(DESTDIR)$(sharedir)/libopenea-cli/
	cp -r EA-CLI/eaml $(DESTDIR)$(sharedir)/libopenea-cli/

