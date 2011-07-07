#!/usr/bin/env python
#encoding: utf-8
from sys import stdin
from optparse import OptionParser
from format import format
from trie import Trie

#candidate entry
class Entry:
    def __init__(self, yomi, distance, word, lid, rid, cost, rank):
        self.yomi = yomi
        self.distance = distance
        self.word = word
        self.lid = lid
        self.rid = rid
        self.cost = cost
        self.rank = rank
    def __str__(self):
        return format((self.word, self.yomi, self.rank))

#spelling corrector class
class SpellingCorrector:
    #load data and init trie
    def __init__(self, dictionary, connection):
        #initalize trie
        self.trie = Trie()
        for line in open(dictionary):
            (yomi, lid, rid, cost, word) = line.strip().split("\t", 4)
            lid, rid, cost = int(lid), int(rid), int(cost)
            yomi, word = unicode(yomi, 'utf-8'), unicode(word, 'utf-8')
            self.trie.insert(yomi, (word, lid, rid, cost))

        #initialize connection
        file = open(connection)
        lsize, rsize = file.readline().strip().split(" ", 1)
        lsize, rsize = int(lsize), int(rsize)
        self.connection = [None] * rsize
        for line in file:
            (lid, rid, cost) = line.strip().split(" ", 2)
            lid, rid, cost = int(lid), int(rid), int(cost)
            if lid != 0:
                break
            self.connection[rid] = cost
        file.close()

    #correct input
    def correct(self, input, threshold=1):
        results = self.trie.fuzzy_search_ex(input, threshold)
        entries = []
        for yomi, distance, values in results:
            for word, lid, rid, cost in values:
                total = cost + self.connection[lid]
                rank = total + (threshold-distance) * 5000
                entry = Entry(yomi, distance, word, lid, rid, cost, rank)
                entries.append(entry)
        entries.sort(key=lambda x:x.rank)
        return entries

if __name__ == '__main__':
    #parse options
    parser = OptionParser()
    parser.add_option("-d", dest="dictionary", default="data/mozc-dictionary.txt")
    parser.add_option("-c", dest="connection", default="data/mozc-connection.txt")
    parser.add_option("-t", dest="threshold", type="int", default=2)
    (options, args) = parser.parse_args()
    corrector = SpellingCorrector(options.dictionary, options.connection)

    #input from stdin
    for line in stdin:
        input = unicode(line.strip(), 'utf-8')
        result = corrector.correct(input, options.threshold)
        for i in result[:50]:
            print format(i)

