all: prac

prac: main.cpp decode.o jaccard.o
	g++ -o prac main.cpp decode.o jaccard.o

decode.o: decode.h
jaccard.o: jaccard.h
