CC = g++
CPPFLAGS = -Wall

all:
	$(CC) $(CPPFLAGS) trie.cc -o trie.out
	$(CC) $(CPPFLAGS) trie_server.cc -o trie_server.out
	$(CC) $(CPPFLAGS) converter.cc -o converter.out
	$(CC) $(CPPFLAGS) stakk.cc -o stakk.out
	$(CC) $(CPPFLAGS) stakk_server.cc -o stakk_server.out
	$(CC) $(CPPFLAGS) test/util_test.cc -o test/util_test.out
	$(CC) $(CPPFLAGS) test/trie_test.cc -o test/trie_test.out
	$(CC) $(CPPFLAGS) test/connection_test.cc -o test/connection_test.out
	$(CC) $(CPPFLAGS) test/stakk_test.cc -o test/stakk_test.out
	$(CC) $(CPPFLAGS) test/converter_test.cc -o test/converter_test.out
	test/util_test.out
	test/trie_test.out
	test/connection_test.out
	test/stakk_test.out
	test/converter_test.out
