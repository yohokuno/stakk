#!/usr/bin/env python
#encoding: utf-8
import sys
from optparse import OptionParser
from format import format

parser = OptionParser()
parser.add_option("-s", dest="separator", default="\t")
parser.add_option("-c", dest="column", type="int", default=-1)
parser.add_option("-k", dest="key", type="int", default=1)
(o, args) = parser.parse_args()

#load dictionary
dictionary = {}
for line in sys.stdin:
    item = line.strip().split(o.separator)
    freq = float(item[o.column])
    key = item[o.key]
    del item[o.column]
    del item[o.key]
    value = item + [freq]
    if not key in dictionary:
        dictionary[key] = [value]
    else:
        dictionary[key].append(value)

#normalize
for key,value in dictionary.items():
    total = sum(item[-1] for item in value)
    dictionary[key] = [item[:-1] + [str(item[-1]/total)] for item in value]

#print format(dictionary)

#output
for key,value in dictionary.items():
    for item in value:
        print key + o.separator + o.separator.join(item)

