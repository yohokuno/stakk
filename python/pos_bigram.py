#!/usr/bin/env python
#encoding: utf-8
from sys import stdin
from optparse import OptionParser
from format import format

#lattice node
class Node:
    def __init__(self, yomi, word, pos, prob, index, total=0.0, back=None):
        self.yomi = yomi
        self.word = word
        self.pos = pos
        self.prob = prob
        self.index = index
        self.total = total
        self.back = back
    def __str__(self):
        return format((self.yomi, self.word, self.pos, self.prob, self.total,self.index,self.back))

#converter class
class Converter:
    #load data
    def __init__(self, file1, file2, separator):
        #initialize dictionary
        self.dictionary = {}
        for line in open(file1):
            (yomi, word, pos, prob) = line.strip().split(separator, 3)
            prob = float(prob)
            self.dictionary.setdefault(yomi, [])
            self.dictionary[yomi].append([yomi, word, pos, prob])

        #initialize connection
        self.connection = {}
        for line in open(file2):
            (left, right, prob) = line.strip().split(separator, 2)
            prob = float(prob)
            key = left + "_" + right
            self.connection[key] = prob

    #convert from kana to kanji
    def convert(self, input, output=False):
        self.input = input
        self.length = len(input)

        #create lattice
        self.lattice = [[] for i in range(self.length+2)]
        self.lattice[0].append(Node(' ', '<S>', '文頭', 1.0, 0, 1.0))
        self.lattice[-1].append(Node(' ', '</S>', '文末', 1.0, 0))
        for i in range(self.length):
            for j in range(i+1, self.length+1):
                yomi = input[i:j]
                for yomi, word, pos, prob in self.dictionary.get(yomi, []):
                    index = len(self.lattice[j])
                    node = Node(yomi, word, pos, prob, index)
                    self.lattice[j].append(node)

        #forward search
        for i in range(1, self.length+2):
            for right in self.lattice[i]:
                j = i - len(right.yomi)
                if len(self.lattice[j]) == 0:
                    break
                def score(left):
                    if left.pos == '文頭': return 1.0
                    if right.pos == '文末': return left.total
                    return left.total * self.connection.get(left.pos+"_"+right.pos, 0.0)
                best = None
                for node in self.lattice[j]:
                    if best == None or score(node) > score(best):
                        best = node
                right.total = right.prob * score(best)
                right.back = best.index
        if output:
            for node in self.lattice: print format(node)

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
    parser.add_option("-d", dest="dictionary", default="data/dictionary.txt")
    parser.add_option("-c", dest="connection", default="data/connection.txt")
    parser.add_option("-s", dest="separator", default="\t")
    parser.add_option("-o", dest="output", action="store_true")
    (options, args) = parser.parse_args()
    converter = Converter(options.dictionary, options.connection, options.separator)

    #input from stdin
    for line in stdin:
        input = line.strip()
        converter.convert(input, options.output)
        print converter.getResult()
        print converter.getCandidates()
        #print format(converter.lattice)

