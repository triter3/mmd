all: prac

prac: main.cpp decode.o jaccard.o minhash.o lsh.o
	g++ -o prac main.cpp decode.o jaccard.o minhash.o lsh.o

decode.o: decode.h
jaccard.o: jaccard.h
minhash.o: minhash.h
lsh.o: lsh.h

clean:
	rm prac *.o
