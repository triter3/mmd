all: prac

prac: main.cpp decode.o jaccard.o minhash.o
	g++ -o prac main.cpp decode.o jaccard.o minhash.o

decode.o: decode.h
jaccard.o: jaccard.h
minhash.o: minhash.h

clean:
	rm prac *.o
