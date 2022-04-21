# Darius-Florentin Neatu <neatudarius@gmail.com>

# Exemplu de Makefile pentru tema

# tag-uri obligatorii (nume + comportament identic)
# build    => compileaza toata tema
#             (ATENTIE! E important - NU compilati in tag-urile de run. Sesizati?)
# run-p$ID => ruleaza problema cu ID-ul specificat (1, 2, 3, 4)
# clean    => sterge toate fisierele generate

# restul este la alegerea studentului
# TODO

# nume necesar (build)
TARGETS=prinel statistics walsh crypto
build:	$(TARGETS)

walsh: walsh.cpp
	g++ walsh.cpp -o walsh

statistics: Statistics.java
	javac $^

prinel: prinel.cpp
	g++ prinel.cpp -o prinel

#prinel: Prinel.java
#	javac $^

crypto: Crypto.java
	javac $^

run-p1:
	./walsh

run-p2:
	java Statistics

run-p3:
	./prinel
#	java Prinel

run-p4:
	java Crypto


clean:
	rm -f *.class $(TARGETS) *.o
