from bs4 import BeautifulSoup
from urllib.request import Request, urlopen
import re
import gzip
import requests
from io import BytesIO
import urllib
from cdl import *


req = Request("http://abel.math.umu.se/~klasm/Data/CONDORCET/MUCDS/")
html_page = urlopen(req)
soup = BeautifulSoup(html_page, "html.parser")

links = []
for link in soup.findAll('a'):
    link = link.get('href')
    if link.startswith("mucds"):
        links.append(link)

server_address = "http://abel.math.umu.se/~klasm/Data/CONDORCET/MUCDS/"


cd = CondorcetDomain(7)

for link in links:
    n = int(link[8])
    if n == cd.n:
        domain = []
        file_address = server_address + link
        mysock = urllib.request.urlopen(file_address)
        memfile = BytesIO(mysock.read())
        with gzip.open(memfile, 'r') as f:
            content = f.read().decode('ascii')
        for line in content.split('\n'):
            if line == '-1':
                ###########################################
                trs = cd.domain_to_trs(domain)


                ###########################################
                domain = []
            else:
                domain.append([int(i)+1 for i in line.split()])

        print(link)