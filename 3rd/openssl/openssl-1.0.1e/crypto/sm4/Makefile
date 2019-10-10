#
# OpenSSL/crypto/sm4/Makefile
#

DIR=	sm4
TOP=	../..
CC=	cc
CPP=	$(CC) -E
INCLUDES=-I$(TOP) -I../../include
CFLAG=-g
MAKEFILE=	Makefile
AR=		ar r
RANLIB=		ranlib
DES_ENC=	sm4_core.o 

CFLAGS= $(INCLUDES) $(CFLAG)
ASFLAGS= $(INCLUDES) $(ASFLAG)
AFLAGS= $(ASFLAGS)

GENERAL=Makefile
#TEST=sm4test.c
APPS=

LIB=$(TOP)/libsg_crypto.a
LIBSRC=sm4_core.c sm4_soft.c

LIBOBJ= sm4_core.o sm4_soft.o

SRC= $(LIBSRC)

EXHEADER= sm4.h
HEADER=	sm4_soft.h $(EXHEADER)

ALL=    $(GENERAL) $(SRC) $(HEADER)

top:
	(cd ../..; $(MAKE) DIRS=crypto SDIRS=$(DIR) sub_all)

all:	lib

lib:	$(LIBOBJ)
	$(AR) $(LIB) $(LIBOBJ)
	$(RANLIB) $(LIB) || echo Never mind.
	@touch lib

# sm4: sm4test.o lib
#	$(CC) $(CFLAGS) -o sm4 sm4test.o $(LIB)

files:
	$(PERL) $(TOP)/util/files.pl Makefile >> $(TOP)/MINFO

links:
	@$(PERL) $(TOP)/util/mklink.pl ../../include/openssl $(EXHEADER)
	@$(PERL) $(TOP)/util/mklink.pl ../../test $(TEST)
	@$(PERL) $(TOP)/util/mklink.pl ../../apps $(APPS)

# We need to use force because 'install' matches 'INSTALL' on case
# insensitive systems
FRC.install:
install: FRC.install
	@[ -n "$(INSTALLTOP)" ] # should be set by top Makefile...
	@headerlist="$(EXHEADER)"; for i in $$headerlist ; \
	do  \
	(cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/include/openssl/$$i; \
	chmod 644 $(INSTALL_PREFIX)$(INSTALLTOP)/include/openssl/$$i ); \
	done;

tags:
	ctags $(SRC)

tests:

lint:
	lint -DLINT $(INCLUDES) $(SRC)>fluff

depend:
	@[ -n "$(MAKEDEPEND)" ] # should be set by upper Makefile...
	$(MAKEDEPEND) -- $(CFLAG) $(INCLUDES) $(DEPFLAG) -- $(PROGS) $(LIBSRC)

dclean:
	$(PERL) -pe 'if (/^# DO NOT DELETE THIS LINE/) {print; exit(0);}' $(MAKEFILE) >Makefile.new
	mv -f Makefile.new $(MAKEFILE)

clean:
	rm -f *.s *.o *.obj des lib tags core .pure .nfs* *.old *.bak fluff

# DO NOT DELETE THIS LINE -- make depend depends on it.

sm4_core.o: ../../include/openssl/sm4.h 
sm4_core.o: ../../include/openssl/e_os2.h ../../include/openssl/opensslconf.h
sm4_core.o: ../../include/openssl/ossl_typ.h ../../include/openssl/safestack.h
sm4_core.o: ../../include/openssl/stack.h ../../include/openssl/symhacks.h
sm4_core.o: ../../include/openssl/ui.h ../../include/openssl/ui_compat.h
sm4_core.o: sm4_core.c 
sm4_soft.o: sm4_soft.c sm4_soft.h
