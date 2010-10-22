#!/usr/bin/env python
#encoding: utf-8

def format(x):
    if type(x) == list:
        result = "["
        result += ', '.join(format(i) for i in x)
        result += "]"
    elif type(x) == tuple:
        result = "("
        result += ', '.join(format(i) for i in x)
        result += ")"
    elif type(x) == dict:
        result = "{"
        result += ', '.join(format(k)+": "+format(v) for k,v in x.items())
        result += "}"
    elif type(x) == str:
        result = "'" + str(x) + "'"
    elif type(x) == float:
        result = "%.2e" % (x)
    elif type(x) == unicode:
        result = x.encode('utf-8')
    else:
        result = str(x)
    return result

if __name__ == '__main__':
    print format([1,2.0,"a"])
    print format((1,2.0,"a"))
    print format({1:1, 2.0:2.0, 'a':'a'})
    print format({"よみ1":[("単語1_1",256),("単語2_2",32)]})
