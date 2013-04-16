ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = reconf configure
SUBDIRS = m4 dg src ana
# add DataGraphics and TrigFTKAna to SUBDIRS to use precompiled
# headers. this is disabled for now because of a bug in gcc
# (http://gcc.gnu.org/bugzilla/show_bug.cgi?id=13675) that causes the
# compiler to die when #include "particular_header_name" appears more
# than once per object file. this bug was fixed as of gcc 4.4.0, but
# it will be a few years before that version appears in Scientific
# Linux. to use, do: SUBDIRS = DataGraphics TrigFTKAna m4 dg plot src ana
