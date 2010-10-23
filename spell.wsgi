#!/usr/bin/env python
#encoding: utf-8
from sys import path
from urllib2 import unquote

directory = "/home/nokuno/stakk"
path.append(directory)
from spell import SpellCorrector

base    = "http://localhost:54633/fuzzy/"
connection = directory + "/data/mozc-connection.txt"
corrector = SpellCorrector(base, connection)

def application(environ, start_response):
    status = '200 OK'
    query = environ['QUERY_STRING'].split('=',1)[1]
    query = unquote(query)
    result = corrector.correct(query, 2)
    output = ""
    for i in result[:50]:
        output += format(i)
    response_headers = [('Content-type', 'text/plain'),
                        ('Content-Length', str(len(output)))]
    start_response(status, response_headers)

    return [output]

if __name__ == '__main__':
    from wsgiref.simple_server import make_server
    make_server('', 808080, application).serve_forever()

