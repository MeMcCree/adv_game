CC=fpc
CUNIT=./raylib-pas/include/
CLIB=./raylib-pas/lib/
CFLAGS=-Fu$(CUNIT) -Fl$(CLIB) -Px86_64
BUILDDIR=./bin/
SRCSDIR=./src/
SRCS=game
SRCSSUFF=$(addsuffix .exe,$(addprefix $(SRCSDIR),$(SRCS)))

.PHONY: build clean

$(CUNIT)/raylib.ppu:
	$(CC) $(CFLAGS) $@ -o$(BUILDDIR)/$*.ppu

$(SRCSDIR)%.exe: $(CUNIT)/raylib.ppu
	$(CC) $(CFLAGS) $(SRCSDIR)/$*.pp -o$(BUILDDIR)/$*.exe

build: $(SRCSSUFF)

clean:
	rm -f $(BUILDDIR)/*.exe
	rm -f $(BUILDDIR)/*.o
	rm -f $(BUILDDIR)/*.ppu