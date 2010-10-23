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

html = """
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>スペル訂正のテスト</title>
<style>
table {
    border-top: 1px solid #ccc;
    border-left: 1px solid #ccc;
}
table td {
    padding: 3px 8px;
    border-bottom: 1px solid #ccc;
    border-right: 1px solid #ccc;
}
</style>
</head>
<body>
<h1><a href="spell">スペル訂正のテスト</a></h1>
<p>
Mozcの辞書を用いたスペル訂正エンジンです。<br>
以下のフォームにひらがなで間違った単語を入力してください。<br>
（例：ぐーｇる、れみおめろん）
</p>
<form>
<input type="text" name="q"/>
<input type="submit" value="訂正">
</form> 
"""
def application(environ, start_response):
    status = '200 OK'
    output = html
    request = environ['REQUEST_URI'].split('?',1)
    if len(request) == 2:
        query = request[1].split("=",1)
        if len(query) == 2:
            output += "<table class='cellpadding='0' cellspacing='0'>";
            output += "<tr><th>訂正候補</th></tr>"
            query = unquote(query[1])
            result = corrector.correct(query, 2)
            for i in result[:20]:
                output += "<tr><td>"
                output += i.word.encode('utf-8')
                output += "</td></tr>"
    output += "</body></html>"
    response_headers = [('Content-type', 'text/html'),
                        ('Content-Length', str(len(output)))]
    start_response(status, response_headers)

    return [output]

if __name__ == '__main__':
    from wsgiref.simple_server import make_server
    make_server('', 808080, application).serve_forever()

