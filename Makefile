all:
	g++ util_test.cc -o util_test
	g++ trie_test.cc -o trie_test
	g++ trie_server.cc -o trie_server
	g++ converter_test.cc -o converter_test
	g++ stakk_test.cc -o stakk_test
	g++ stakk_server.cc -o stakk_server
