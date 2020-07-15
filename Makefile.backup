.PHONY: clean functional-test performance-test

# Set the default target. When you make with no arguments,
# this will be the target built.
COMPILER = compiler
PRODUCTS = $(COMPILER)
default: $(PRODUCTS)

# Set up the list of source and object files
SRCS = GrammarTree.c Nonterminals.c main.c

# OBJS can deal with either .cc or .c files listed in SRCS
OBJS = y.tab.o lex.yy.o $(patsubst %.cc, %.o, $(filter %.cc,$(SRCS))) $(patsubst %.c, %.o, $(filter %.c, $(SRCS)))

JUNK = *.o lex.yy.c y.tab.c y.tab.h

LEX_SRC = scanner.l

YACC_SRC = parser.y

# Define the tools we are going to use
CC = gcc
LD = gcc
LEX = flex
YACC = bison

# Set up the necessary flags for the tools
EXTRA_CFLAGS = 

ifdef opt
EXTRA_CFLAGS = -DDCC_OPTIMIZATION
endif

# We want debugging and most warnings, but lex/yacc generate some
# static symbols we don't use, so turn off unused warnings to avoid clutter
# Also STL has some signed/unsigned comparisons we want to suppress
CFLAGS = -g -Wall -Wno-unused -Wno-sign-compare

# The -d flag tells lex to set up for debugging. Can turn on/off by
# setting value of global yy_flex_debug inside the scanner itself
# The -L flag makes lex.yy.c able to be debugged.
LEXFLAGS = -dL

# The -d flag tells yacc to generate header with token types
# The -v flag writes out a verbose description of the states and conflicts
# The -t flag turns on debugging capability
# The -y flag means imitate yacc's output file naming conventions
YACCFLAGS = -dvty

# Link with standard C library, math library, and lex library
LIBS = -lc -lm -ll

# Rules for various parts of the target

y.tab.h y.tab.c: $(YACC_SRC)
	$(YACC) $(YACCFLAGS) $(YACC_SRC)

lex.yy.c: $(LEX_SRC) y.tab.h y.tab.c
	$(LEX) $(LEXFLAGS) -o $@ $(LEX_SRC) 

.o: $*.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $*.c

# rules to build compiler (dcc)
.DELETE_ON_ERROR:
$(COMPILER): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)


# This target is to build small for testing (no debugging info), removes
# all intermediate products, too
strip : $(PRODUCTS)
	strip $(PRODUCTS)
	rm -rf $(JUNK)

clean:
	rm -f $(JUNK) y.output $(PRODUCTS)

