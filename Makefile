SOURCES := jstring.c
OBJECTS := $(subst .c,.o,$(SOURCES))
HEADERS := $(subst .c,.h,$(SOURCES))

DYN_SHARED_LIB 	:= libjstring.so
STATIC_LIB 		:= libjstring.a

LDFLAGS := -shared
RANLIB  := ranlib

CFLAGS += -Wall -pedantic -fPIC

DEBUG_CFLAGS := -g -O0
CFLAGS += $(DEBUG_CFLAGS)

DEBUG_LIB_DEP_DIR := ../debug
CFLAGS += $(addprefix -I,$(DEBUG_LIB_DEP_DIR))

.PHONY: all clean test

all: $(DYN_SHARED_LIB) $(STATIC_LIB)

$(DYN_SHARED_LIB): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(STATIC_LIB): $(OBJECTS) $(HEADERS)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $<
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(DYN_SHARED_LIB) $(STATIC_LIB)

test: