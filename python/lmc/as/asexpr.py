#!/usr/bin/python


def doToken (e, t):
    f = []
    for i in e:
        if i != "":
            f += i.split (t)
    g = []
    for i in f:
        if i != "":
            g += [i]
    return g

def doLex (s):
    e = s.split (' ')
    e = doToken (e, '+')
    e = doToken (e, '-')
    e = doToken (e, '/')
    e = doToken (e, '*')
    e = doToken (e, '(')
    e = doToken (e, ')')
    e = doToken (e, '%')
    e = doToken (e, '.')
    e += ['<eof>']
    return e

class expression:
    def __init__ (self, s, e, o, l):
        self.original = s
        self.tokenList = doLex (s)
        self.error = e
        self.currentToken = self.tokenList[0]
        self.resolved = False
        self.value = 0
        self.origin = o
        self.lookup = l

    def eof (self):
        return self.currentToken == '<eof>'
    
    def isToken (self, t):
        return self.currentToken == t

    def isNumber (self):
        return self.currentToken[0].isdigit ()

    def expect (self, t):
        if self.isToken (t):
            self.nextToken ()
        else:
            self.error ('expecting ' + t + ' in expression')

    def isName (self):
        return self.currentToken.isalpha ()

    def resolve (self):
        if not self.resolved:
            self.value = self.doExpression ()
            self.resolved = True
        return self.value

    def nextToken (self):
        self.tokenList = self.tokenList[1:]
        self.currentToken = self.tokenList[0]

    def doExpression (self):
        c0 = self.doUnaryOrConstTerm ()
        while self.isToken ('-') or self.isToken ('+'):
            if self.isToken ('-'):
                self.expect ('-')
                c0 -= self.doUnaryOrConstTerm ()
            else:
                self.expect ('+')
                c0 += self.doUnaryOrConstTerm ()
        return c0

    def doUnaryOrConstTerm (self):
        if self.isToken ('+'):
            self.expect ('+')
            return self.doConstTerm ()
        elif self.isToken ('-'):
            self.expect ('-')
            return -self.doConstTerm ()
        else:
            return self.doConstTerm ()

    def doConstTerm (self):
        c = self.doConstFactor ()
        while self.isToken ('*') or self.isToken ('/') or self.isToken ('%'):
            if self.isToken ('*'):
                self.expect ('*')
                c *= self.doConstFactor ()
            elif self.isToken ('/'):
                self.expect ('/')
                c /= self.doConstFactor ()
            elif self.isToken ('%'):
                self.expect ('%')
                c %= self.doConstFactor ()
        return c

    def doConstFactor (self):
        if self.isNumber ():
            return self.doNumber ()
        elif self.isToken ('.'):
            return self.origin
        elif self.isName ():
            return self.lookupName ()
        elif self.isToken ('('):
            self.expect ('(')
            e = self.doExpression ()
            self.expect (')')
            return e
        else:
            self.error ('syntax error in expression: ' + self.original + ' near ' + self.currentToken)
            return 1

    def lookupName (self):
        return self.lookup (self.currentToken)

    def doNumber (self):
        if self.currentToken.isdigit():
            return int (self.currentToken)
        elif (len (self.currentToken) > 1) and (self.currentToken[1] == 'x'):
            return self.doHex (self.currentToken[2:])
        else:
            self.error ('invalid number: ' + self.currentToken)
            return 1

    def doHexDigit (self, h):
        if h.isdigit ():
            return int (h)
        elif h in ['a', 'b', 'c', 'd', 'e']:
            return ord (h) - ord ('a') + 10
        else:
            self.error ('invalid hex number: ' + self.currentToken)
            return 1

    def doHex (self, h):
        v = self.doHexDigit (h[0])
        i = 1
        while i < len (h):
            v = v * 16 + self.doHexDigit (h[i])
            i += 1
        return v

    def isNumber (self):
        if self.currentToken.isdigit ():
            return True
        return (len (self.currentToken) > 2) and self.currentToken[:2] == '0x'


