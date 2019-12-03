import sys
import base64
import gzip

def main():
    package = sys.argv[1]
    if ('*.py' in package):
        fi = gzip.open('./PAYLOADZ', 'wb')
        pack = open(package, 'rb')
        lines = pack.readlines()
        fi.writelines(lines)
        pack.close()
        compress('./PAYLOADZ')


def compress(dfile):
    f = open(dfile, 'rb')
    n = open('./PAYLOADZ', 'wb')
    lines = f.readlines()
    nlines = []
    for line in lines:
        nlines.append(base64.b64encode(line))

    n.writelines(nlines)

def decomp(cfile):
    f = open(cfile, 'rb')
    n = open('./Packed', 'wb')

    lines = f.readlines()
    nlines = []
    for line in lines:
        nlines.append(base64.b64decode(line))
    n.writelines(nlines)
    
main()
