#!/usr/bin/env python
#encoding: utf-8
from sys import stdin
from optparse import OptionParser
from format import format
from urllib import urlopen, quote

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
        return format((self.word, self.yomi, self.distance, self.cost, self.rank))

class SpellCorrector:
    #initialize
    def __init__(self, base, connection):
        self.base = base

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
        url = self.base + quote(input)
        results = []
        for line in urlopen(url):
            (yomi, distance, lid, rid, cost, word) = line.strip().split("\t", 5)
            lid, rid, cost, distance = int(lid), int(rid), int(cost), int(distance)
            yomi, word = unicode(yomi, 'utf-8'), unicode(word, 'utf-8')
            results.append((yomi, distance, lid, rid, cost, word))

        entries = []
        for yomi, distance, lid, rid, cost, word in results:
            total = cost + self.connection[lid]
            rank = total + (threshold-distance) * 5000
            entry = Entry(yomi, distance, word, lid, rid, cost, rank)
            entries.append(entry)
        entries.sort(key=lambda x:x.rank)
        return entries

if __name__ == '__main__':
    #parse options
    parser = OptionParser()
    parser.add_option("-b", dest="base", default="http://localhost:54633/fuzzy/")
    parser.add_option("-c", dest="connection", default="data/mozc-connection.txt")
    parser.add_option("-t", dest="threshold", type="int", default=2)
    (options, args) = parser.parse_args()
    corrector = SpellCorrector(options.base, options.connection)

    #input from stdin
    for line in stdin:
        input = line.strip()
        result = corrector.correct(input, options.threshold)
        for i in result[:50]:
            print format(i)

