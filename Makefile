########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lc -lm -ll

# Makefile settings - Can be customized.
APPNAME = compiler
EXT = .cpp
SRCDIR = .
OBJDIR = .

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)
# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ) parser.tab.o lex.yy.o
	$(CC) $(CXXFLAGS) -o $@ parser.tab.o lex.yy.o $^ $(LDFLAGS) # ! Add lex.yy.o and parser.tab.o

# Creates the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Building rule for yacc and lex files
lex.yy.c: parser.tab.c parser.tab.h
	flex -L scanner.l

parser.tab.c, parser.tab.h:
	bison -dvt parser.y

parser.tab.o, lex.yy.o: parser.tab.c parser.tab.h lex.yy.c
	# ! Compile yacc and lex files
	g++ -Wall -Wno-unused -Wno-sign-compare -c parser.tab.c
	g++ -Wall -Wno-unused -Wno-sign-compare -c lex.yy.c

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT) parser.tab.h
	$(CC) $(CXXFLAGS) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)
	# ! rm *.tab.c *.tab.h and lex.yy.c
	$(RM) parser.tab.c parser.tab.h parser.output
	$(RM) lex.yy.c

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)