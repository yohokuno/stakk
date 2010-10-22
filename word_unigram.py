#!/usr/bin/env python
#encoding: utf-8
import sys
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-f", dest="file", default="data/yomi_word_unigram.txt")
(o, args) = parser.parse_args()

#load dictionary
dic = {}
summation = 0
for line in open(o.file):
    (yomi, surf, freq) = line.strip().split("\t", 2)
    freq = int(freq)
    if not yomi in dic:
        dic[yomi] = (surf, freq)
    summation += freq

#input from stdin
for line in sys.stdin:
    input = line.strip()
    length = len(input)
    #create lattice
    lattice = [None] * (length+1)
    lattice[0] = ('<S>', '', 1.0)
    for i in range(length):
        for j in range(i+1, length+1):
            yomi = input[i:j]
            if yomi in dic and lattice[i] != None:
                (surf, freq) = dic[yomi]
                prob = lattice[i][2] * freq / summation
                if lattice[j] == None or lattice[j][1] < prob:
                    lattice[j] = (surf, yomi, prob)
    #back trace
    i = length
    result = ""
    while i > 0 and lattice[i] != None:
        word = lattice[i]
        result = word[0] + " " + result
        i -= len(word[1])
    print result
