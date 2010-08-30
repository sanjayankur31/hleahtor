bin_PROGRAMS = hleahtor
hleahtor_SOURCES = checkUpdate.c  configParser.c  configParser.h  \
			   hleahtor.c  hleahtor.h hleahtorCommon.h checkUpdate.h hleahtorNotify.c \
			   hleahtorNotify.h

# AM_CFLAGS = 
hleahtor_CFLAGS = -DDATADIR=\"$(datadir)\"  @PACKAGE_CFLAGS@
hleahtor_LDFLAGS = @PACKAGE_LIBS@
