#!/usr/bin/env python
#encoding: utf-8
from sys import stdin
from optparse import OptionParser
from format import format

class Trie:
    def __init__(self, data=[]):
        self.children = {}
        self.values = []
        for key, value in data:
            self.insert(key, value)

    def insert(self, key, value):
        if key:
            first, rest = key[0], key[1:]
            if first not in self.children:
                self.children[first] = Trie()
            self.children[first].insert(rest, value)
        else:
            self.values.append(value)

    def common_prefix_search(self, key):
        results = []
        if self.values:
            results = [('', self.values)]
        if not key or not self.children:
            return results
        first, rest = key[0], key[1:]
        if first in self.children:
            children = self.children[first].common_prefix_search(rest)
            results.extend((first + k,v) for k,v in children)
        return results

    def predictive_search(self, key):
        results = []
        if not self.children:
            return results
        if key:
            first, rest = key[0], key[1:]
            if first in self.children:
                children = self.children[first].predictive_search(rest)
                results.extend((first + k,v) for k,v in children)
        else:
            if self.values:
                results.append(('', self.values))
            for k1, v1 in self.children.items():
                children = v1.predictive_search(key)
                results.extend((k1+k,v) for k,v in children)
        return results

    def search(self, key):
        result = None
        if not key and self.values:
            return self.values
        if not key or not self.children:
            return None
        first, rest = key[0], key[1:]
        if first in self.children:
            result = self.children[first].search(rest)
        return result
        
    def fuzzy_search(self, key, distance=1):
        results = []
        if not key and self.values:
            results = [('', distance, self.values)]
        if not self.children:
            return results

        # normal match
        if key:
            first, rest = key[0], key[1:]
            if first in self.children:
                children = self.children[first].fuzzy_search(rest, distance)
                results.extend((first + k,d,v) for k,d,v in children)
        
        # edit operation
        if distance > 0:
            # insert
            for k1, v1 in self.children.items():
                children = v1.fuzzy_search(key, distance-1)
                results.extend((k1+k,d,v) for k,d,v in children)

            # delete
            if key:
                children = self.fuzzy_search(key[1:], distance-1)
                results.extend(children)

            # substitution
            if key:
                for k1, v1 in self.children.items():
                    if k1 != key[0]:
                        children = v1.fuzzy_search(key[1:], distance-1)
                        results.extend((k1+k,d,v) for k,d,v in children)

            # transpose
            if len(key) > 1:
                new_key = key[1] + key[0] + key[2:]
                children = self.fuzzy_search(new_key, distance-1)
                results.extend(children)
                    
        return results

    def fuzzy_search_ex(self, key, distance=2):
        results = self.fuzzy_search(key, distance)
        dictionary = {}
        for k, d, v in results:
            if not k in dictionary or d > dictionary[k][0]:
                dictionary[k] = (d, v)
        result = [(k, d, v) for k, (d,v) in dictionary.items()]
        result.sort(key=lambda x:x[1], reverse=True)
        return result

    def display(self, key="", depth=0):
        if self.values:
            print key + "\t" + format(self.values)
        for (k,v) in self.children.items():
            v.display(key + k, depth+1)

if __name__ == '__main__':
    #parse option
    parser = OptionParser()
    parser.add_option("-f", dest="filename")
    parser.add_option("-s", dest="separator", default="\t")
    parser.add_option("-d", dest="distance", type="int", default=1)
    parser.add_option("-m", dest="mode", default="all")
    (options, args) = parser.parse_args()

    if options.filename != None:
        trie = Trie()
        for line in open(options.filename):
            key, value = line.strip().split(options.separator, 1)
            key, value = unicode(key, 'utf-8'), unicode(value, 'utf-8')
            trie.insert(key, value)

        for line in stdin:
            input = unicode(line.strip(), 'utf-8')
            # common prefix search
            if options.mode in ('all', 'common'):
                result = trie.common_prefix_search(input)
                print 'common prefix:'
                for i in result:
                    print format(i)
            # predictive search
            if options.mode in ('all', 'predict'):
                result = trie.predictive_search(input)
                print 'predict:'
                for i in result:
                    print format(i)
            # fuzzy search
            if options.mode in ('all', 'fuzzy'):
                result = trie.fuzzy_search_ex(input, options.distance)
                print 'fuzzy:'
                for i in result:
                    print format(i)

    else:
        trie = Trie()
        trie.insert(u'わたし',5)
        trie.insert(u'わし',6)
        trie.display()
        print 'fuzzy: ', format(trie.fuzzy_search(u'わし'))

        trie = Trie()
        trie.insert('try', 1)
        trie.insert('tryed', 2)
        trie.insert('tryes', 3)
        trie.insert('try', 4)
        trie.display()
        print 'common prefix: ', trie.common_prefix_search('tryes')
        print 'common prefix: ', trie.common_prefix_search('try')
        print 'common prefix: ', trie.common_prefix_search('tryis')
        print 'common prefix: ', trie.common_prefix_search('tr')
        print 'common prefix: ', trie.common_prefix_search('r')
        print 'predict: ', trie.predictive_search('trye')
        print 'predict: ', trie.predictive_search('t')
        print 'predict: ', trie.predictive_search('r')
        print 'search: ', trie.search('try')
        print 'search: ', trie.search('tryed')
        print 'search: ', trie.search('trye')
        print 'fuzzy: ', trie.fuzzy_search('try')
        print 'fuzzy: ', trie.fuzzy_search('ty')
        print 'fuzzy: ', trie.fuzzy_search('tay')
        print 'fuzzy: ', trie.fuzzy_search('tray')
        print 'fuzzy: ', trie.fuzzy_search('trye')
        print 'fuzzy: ', trie.fuzzy_search('tyr')

