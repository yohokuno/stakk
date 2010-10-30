#!/usr/bin/env python
#encoding: utf-8
from sys import stdin
from optparse import OptionParser
from urllib import urlopen, quote
from format import format

#lattice node
class Node:
    def __init__(self, line):
        data = line.strip().split("\t")
        self.yomi = data[0]
        self.lid = int(data[2])
        self.rid = int(data[3])
        self.cost = int(data[4])
        self.word = data[5]
        self.index = 0
        self.total = 0
        self.back = None
    def __str__(self):
        return format((self.yomi, self.word, self.lid, self.rid, self.cost, self.total, self.index, self.back))

#converter class
class Converter:
    #load data
    def __init__(self, url, connection):
        self.url = url

        #initialize connection
        file = open(connection)
        lsize, rsize = file.readline().strip().split(" ", 1)
        lsize, rsize = int(lsize), int(rsize)
        self.connection = [None] * lsize
        for i in range(lsize):
            self.connection[i] = [None] * rsize
        for line in file:
            (lid, rid, cost) = line.strip().split(" ", 2)
            lid, rid, cost = int(lid), int(rid), int(cost)
            self.connection[lid][rid] = cost
        file.close()

    #convert from kana to kanji
    def convert(self, input, output=False):
        self.input = input
        self.length = len(input)

        #create lattice
        self.lattice = [[] for i in range(self.length+2)]
        self.lattice[0].append(Node("S\t0\t0\t0\t0\tBOS"))
        self.lattice[-1].append(Node("S\t0\t0\t0\t0\tBOS"))
        for i in range(self.length):
            query = self.url + quote(input[i:])
            for line in urlopen(query):
                node = Node(line)
                j = i + len(node.yomi)
                node.index = len(self.lattice[j])
                self.lattice[j].append(node)

        #forward search
        for i in range(1, self.length+2):
            for right in self.lattice[i]:
                j = i - len(right.yomi)
                if len(self.lattice[j]) == 0:
                    break
                def score(left):
                    if left.rid == 0: return 0
                    if right.lid == 0: return left.total
                    return left.total + self.connection[left.rid][right.lid]
                best = None
                for node in self.lattice[j]:
                    if best == None or score(node) < score(best):
                        best = node
                right.total = right.cost + score(best)
                right.back = best.index
        if output:
            for node in self.lattice:
                print format(node)

        #back trace
        current = self.length
        position = self.lattice[self.length+1][0].back
        self.result = []
        while current > 0:
            node = self.lattice[current][position]
            self.result.insert(0, node)
            position = node.back
            current -= len(node.yomi)

    #get string result
    def getResult(self):
        return ' '.join(node.word for node in self.result)

    #create word-based candidates list
    def createCandidates(self):
        current = 0
        self.candidates = []
        for node in self.result:
            current += len(node.yomi)
            words = [node.word]
            for cand in self.lattice[current]:
                if cand != node and len(cand.yomi) == len(node.yomi):
                    words.append(cand.word)
            self.candidates.append(words)

    #get candidate string
    def getCandidates(self):
        self.createCandidates()
        result = ""
        for nodes in self.candidates:
            result += ' '.join(node for node in nodes) + "\n"
        return result

if __name__ == '__main__':
    #parse options
    parser = OptionParser()
    parser.add_option("-u", dest="url", default="http://localhost:54633/common/")
    parser.add_option("-c", dest="connection", default="data/connection.txt")
    parser.add_option("-o", dest="output", action="store_true")
    (options, args) = parser.parse_args()
    print 'loading..'
    converter = Converter(options.url, options.connection)
    print 'input kana: '

    #input from stdin
    for line in stdin:
        input = line.strip()
        converter.convert(input, options.output)
        print converter.getResult()
        print converter.getCandidates()
        #print format(converter.lattice)

