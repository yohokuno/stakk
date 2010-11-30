CC = g++
CPPFLAGS = -Wall

all:
	$(CC) $(CPPFLAGS) util_test.cc -o util_test
	$(CC) $(CPPFLAGS) trie_test.cc -o trie_test
	$(CC) $(CPPFLAGS) trie_server.cc -o trie_server
	$(CC) $(CPPFLAGS) converter_test.cc -o converter_test
	$(CC) $(CPPFLAGS) stakk_test.cc -o stakk_test
	$(CC) $(CPPFLAGS) stakk_server.cc -o stakk_server
