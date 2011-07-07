#!/usr/bin/python
#encoding: utf-8
import sys

for line in sys.stdin:
    (freq, words) = line.strip().split(" ", 1)
    ngram = words.split(" ")
    pair = [word.split(",",1) for word in ngram]
    if any(len(p) != 2 for p in pair):
        continue
    surf = " ".join(p[0] for p in pair)
    yomi = "".join(p[1] for p in pair)
    print "%s\t%s\t%s" % (yomi, surf, freq)

