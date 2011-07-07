#!/usr/bin/env python
#encoding: utf-8
from sys import stdin
from optparse import OptionParser
from format import format
from kana import katakana2hiragana

parser = OptionParser()
parser.add_option("-s", dest="separator", default="\t")
(o, args) = parser.parse_args()

#marginalize
def marginalize(dictionary, column):
    result = {}
    for key, value in dictionary.items():
        k = key[:column] + key[column+1:]
        result[k] = result.get(k,0.0) + value
    return result

#normalize
def normalize(dictionary, column):
    total = {}
    for key, value in dictionary.items():
        k = key[column]
        total[k] = total.get(k,0.0) + value
    result = {}
    for key, value in dictionary.items():
        k = key[column]
        result[key] = value / total[k]
    return result

#load dictionary
dictionary = {}
for line in stdin:
    (yomi, word, pos, freq) = line.strip().split(o.separator, 3)
    freq = float(freq)
    yomi = katakana2hiragana(yomi)
    dictionary[(yomi,word,pos)] = float(freq)

#create conditionary distribution
word_pos = marginalize(dictionary, 0) #marginalize on yomi
word_pos = normalize(word_pos, 1)   #normalize given pos

#print format(word_pos.items()[:10])

yomi_word = marginalize(dictionary, 2) #marginalize on pos
yomi_word = normalize(yomi_word, 1) #normalize given word

#print format(yomi_word.items()[:10])

for key,value in dictionary.items():
    (yomi, word, pos) = key
    probability = word_pos[(word,pos)] * yomi_word[(yomi,word)]
    print "\t".join(key) + "\t" + str(probability)

