# Dependency rules for non-file targets
all: testsymtablelist testsymtablehash
clobber: clean
rm -f *~ \#*\#
clean:
rm -f testsymtablelist testsymtablehash *.o
# Dependency rules for file targets
testsymtablelist: symtablelist.o testsymtablelist.o
gcc217 symtablelist.o testsymtable.o –o testsymtablelist
testsymtablelist.o: testsymtable.c symtable.h
gcc217 -c testsymtable.c

testsymtablehash: symtablehash.o testsymtablehash.o
gcc217 symtablehash.o testsymtable.o –o testsymtablehash
testsymtablehash.o: testsymtable.c symtable.h
gcc217 -c testsymtable.c
