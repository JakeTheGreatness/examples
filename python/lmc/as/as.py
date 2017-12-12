#!/usr/bin/python

import os
import getopt
import sys
import tempfile

from asexpr import expression


listing = False
extended = True
verbose = False
outputFile = "-"
inputFile = "-"
programName = "aslmc"
maxMemory = 256
passNumber = 1


def usage (code):
    printf ("usage: %s [-o outputfile] [-hlvx] inputfile\n", programName)
    printf ("   -h   help\n")
    printf ("   -l   produce a listing\n")
    printf ("   -r   reduced\n")
    printf ("   -v   verbose\n")
    printf ("   -x   extended\n")
    os.sys.exit (code)


#
#  handleArguments -
#

def handleArguments ():
    global inputFile, listing, extended, reduced, verbose

    try:
        optlist, list = getopt.getopt(sys.argv[1:],':hlo:rvx')
    except getopt.GetoptError:
        usage (1)
    for opt in optlist:
        if opt[0] == '-h':
            usage(0)
        if opt[0] == '-o':
            outputFile = opt[1]
        if opt[0] == '-l':
            listing = True
        if opt[0] == '-x':
            extended = True
        if opt[0] == '-r':
            reduced = True
        if opt[0] == '-v':
            verbose = True

    inputFile = list[0]


instructions = { "add":"1xx",
                 "sub":"2xx",
                 "sta":"3xx",
                 "lca":"4xx",   # load constant to accumulator
                 "lda":"5xx",
                 "bra":"6xx",
                 "brz":"7xx",
                 "brp":"8xx",
                 "inp":"901",
                 "out":"902",
                 "hlt":"000",
                 "cob":"000",
                 "dat":"xxx" }

def errorOrigin (message):
    syntaxError ('.origin which must contain a resolvable expression', message)

def doOrigin (words):
    global origin
    origin = expression (words[1], errorOrigin, origin, lookupLabel).resolve ()       

def tohexchar (v):
    if v < 10:
        return chr (ord('0') + v)
    else:
        return chr (ord('a') + v -10)

def dohex (v):
    if v < 16:
        return tohexchar (v)
    else:
        return dohex (v/16) + tohexchar (v % 16)

def doString (words):
    global origin, passNumber

    s = " ".join (words[1:])
    if s[0] == '"':
        w = s.split('"')[1]
    elif s[0] == "'":
        w = s.split("'")[1]
    for c in w:
        if passNumber == 2:
            assignMemory ('.string ' + s, dohex (ord (c)))
        origin = (origin + 1) % maxMemory


directives = { ".origin":doOrigin,
               ".string":doString }

labels = {}
memory = {}

origin = 0
lineno = 0


def scanner (name, function):
    global lineno
    fd = open (name, 'r')
    while True:
        line = fd.readline ()
        lineno += 1
        if not line:
            break
        function (line)
    fd.close ()


#
#  printf - keeps C programmers happy :-)
#

def printf (format, *args):
    print str(format) % args,


def syntaxError (line, message):
    global lineno, inputFile
    printf ("%s:%d: error in %s\n", inputFile, lineno, line)
    printf ("%s:%d:%s\n", inputFile, lineno, message)


def doDirective (w):
    if directives.has_key (w[0]):
        f = directives[w[0]]
        f (w)

def doMnemonic (line, w):
    if instructions.has_key (w[0]):
        k = instructions[w[0]]
        processOperand (line, k, w)


def assignMemory (line, k):
    global memory, origin, listing

    if k == None:
        return
    if memory.has_key (origin):
        old = memory[origin]
        if old[0] == "":
            memory[origin] = [k, []]
        else:
            syntaxError (line, 'already assigned data or code to this location')
    else:
        memory[origin] = [k, []]
    if listing:
        printAddress (origin)
        printf ("%s     %s\n", k, line)


def doEncode (line, k, c):
    e = ''
    for i in k[::-1]:
        if i == 'x':
            e = '%x' % (c % 16) + e
            c /= 16
        else:
            e = i + e
    return e


def errorEncode (message):
    syntaxError ('encoding words', message)


def encodeWord (line, k, w):

    if len (w) <= 1:
        return doEncode (line, k, 0)
    else:
        c = expression (w[1], errorEncode, origin, lookupLabel).resolve ()
        return doEncode (line, k, c)


#
#
#

def processOperand (line, k, w):
    global origin, passNumber

    if passNumber == 2:
        if k.find ('x') >= 0:
            assignMemory (line, encodeWord (line, k, w))
        else:
            assignMemory (line, k)
    origin = (origin + 1) % maxMemory


#
#
#

def processMnemonic (line):
    line = line.rstrip ()
    if line != "":
        w = line.split ()
        if directives.has_key (w[0]):
            doDirective (w)
        elif instructions.has_key (w[0]):
            doMnemonic (line, w)
        else:
            syntaxError (line, 'neither a legal directive or mnemonic')


def doPreprocessor (line):
    pass


def isWhite (ch):
    return ch == ' ' or ch == '\t'


def lookupLabel (l):
    global labels

    if labels.has_key (l):
        return labels[l]
    else:
        syntaxError (l, 'undeclared label')
        return None

def processLabel (line):
    global labels
    labels[origin] = line
    labels[line] = origin


def processFragment (line):
    global passNumber

    line = line.rstrip ()
    w = line.split(':')
    if len (w) > 1:
        #  label found
        if passNumber == 1:
            processLabel (w[0])
        line = " ".join (w[1:])
        processMnemonic (line)
    else:
        processMnemonic (line)


def skipOver (line, i, c):
    i += 1
    while i < len (line):
        if line[i] == c:
            return i
        i += 1
    return i


def splitOnSemi (line):
    start = 0
    i = 0
    while i < len (line):
        if line[i] == '"':
            i = skipOver (line, i, '"')
        elif line[i] == "'":
            i = skipOver (line, i, "'")
        elif line[i] == ';':
            processFragment (line[start:i])
            start = i+1
        i += 1
    if start < len (line):
        processFragment (line[start:])


#
#  passOne -
#

def passOne (line):
    global passNumber, verbose

    passNumber = 1
    doPass (line)


#
#  passTwo -
#

def passTwo (line):
    global passNumber

    passNumber = 2
    doPass (line)


#
#  doPass -
#

def doPass (line):
    if len (line) > 0:
        if line[0] == '#':
            doPreprocessor (line)
        else:
            line = line.split ('#')[0]  # remove comments like this!
            if len (line) > 0:
                splitOnSemi (line)


def parseFile ():
    global inputFile, origin

    fd, name = tempfile.mkstemp('as')
    os.system ('gcc -E ' + inputFile + ' -o ' + name)
    origin = 0
    if verbose:
        printf ("Pass 1:  labels\n")
    scanner (name, passOne)
    origin = 0
    if verbose:
        printf ("Pass 2:  mnemonics, operands and expressions\n")
    scanner (name, passTwo)


def printAddress (a):
    if a < 16:
        printf ("0x00%x:  ", a)
    elif a < 256:
        printf ("0x0%x:  ", a)
    else:
        printf ("0x%x:  ", a)

def dumpHex (c):
    if len (c) == 1:
        printf ("0x00%s ", c)
    elif len (c) == 2:
        printf ("0x0%s ", c)
    elif len (c) == 3:
        printf ("0x%s ", c)
    else:
        printf ("xxxx ")


def dumpMemory ():
    printf ("# memory dump\n")
    i = 0
    keyed = False
    for a in range (maxMemory):
        if memory.has_key (a):
            if not keyed:
                printAddress (a)
                keyed = True
            c = memory[a]
            dumpHex (c[0])
            i += 1
            if i % 8 == 0:
                printf ("\n")
                keyed = False
        else:
            keyed = False
    printf ("\n")
            

#
#
#

def main ():
    handleArguments ()
    parseFile ()
    dumpMemory ()


main ()

