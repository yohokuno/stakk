CC = g++
CCFLAGS = -Wall
.PHONY: all test clean pack

all:
	$(CC) $(CCFLAGS) trie.cc -o trie.out
	$(CC) $(CCFLAGS) trie_server.cc -o trie_server.out
	$(CC) $(CCFLAGS) converter.cc -o converter.out
	$(CC) $(CCFLAGS) stakk.cc -o stakk.out
	$(CC) $(CCFLAGS) stakk_server.cc -o stakk_server.out

test:
	$(CC) $(CCFLAGS) test/utrie_test.cc -o test/utrie_test.out
	test/utrie_test.out
	$(CC) $(CCFLAGS) test/ustring_test.cc -o test/ustring_test.out
	test/ustring_test.out
	$(CC) $(CCFLAGS) test/util_test.cc -o test/util_test.out
	test/util_test.out
	$(CC) $(CCFLAGS) test/trie_test.cc -o test/trie_test.out
	test/trie_test.out
	$(CC) $(CCFLAGS) test/connection_test.cc -o test/connection_test.out
	test/connection_test.out
	$(CC) $(CCFLAGS) test/stakk_test.cc -o test/stakk_test.out
	test/stakk_test.out
	$(CC) $(CCFLAGS) test/converter_test.cc -o test/converter_test.out
	test/converter_test.out

clean:
	rm *.out */*.out

pack:
	tar cfj stakk.tar.bz2 `find . -type f | egrep -v ".git|.out|.tar"` 
