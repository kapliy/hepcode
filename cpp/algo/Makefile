.PHONY : all

all: run tree hanoi hash

run: algo.cpp
	g++ -g algo.cpp -o $@

tree: tree.cpp
	g++ -g tree.cpp -o $@

hash: hash.cpp
	g++ -g hash.cpp -o $@

hanoi: Hanoi.cpp
	g++ -g Hanoi.cpp -o $@

.PHONY : clean
clean:
	@rm -f a.out run tree hanoi hash
