#!/usr/bin/env python3

import os, pygame, math
from pygame.locals import *
import random
from gameSound import playSound

SplayDrop  = 10


#
#
#

class cardPile:
    def __init__ (self, cards=[], pos=[0, 0], splay=[SplayDrop, SplayDrop], cVec=[0, 0]):
        self.cards = cards
        self.splay = splay
        print("pile at", pos)
        self.pos = pos
        self.collectVec = cVec
        self.cardsUp = True
        self.selected = -1
        if cVec != None:
            p = list(pos)
            for c in cards:
                c.putPos(p)
                p = [p[0]+cVec[0], p[1]+cVec[1]]

    def shuffle (self):
        random.shuffle (self.cards)

    def _shuffle (self, surface, cloth):
        if not self.cardsUp:
            if len (self.cards)>1:
                playSound('shuffle')
                self.shuffle ()
                pygame.time.delay (3000)
            self.collect (surface, cloth, self.collectVec)

    def pickUp (self, surface, cloth, delay=0):
        if not self.cardsUp:
            self._pickUp (surface, cloth, delay)
        self.cardsUp = True

    def _pickUp (self, surface, cloth, delay=0):
        for c in self.cards:
            p = c.getPos ()
            imagepos = c.getRect ()
            imagepos.left = p[0]
            imagepos.top  = p[1]
            pygame.draw.rect (surface, cloth, imagepos)
            if delay != 0:
                pygame.time.delay (delay)
                pygame.display.flip ()

    def dropDown (self, surface):
        if self.cardsUp:
            for c in self.cards:
                i = random.randint(0, self.splay[0])
                j = random.randint(0, self.splay[1])
                p = c.getPos ()
                imagepos = c.getRect ()
                p = [p[0]+i, p[1]+j]
                imagepos.left = p[0]
                imagepos.top  = p[1]
                c.putPos (p)
                playSound('cardsnap')
                surface.blit (c.getImage (), imagepos)
                pygame.display.flip ()
                pygame.time.delay (300)
        self.cardsUp = False

    def move (self, surface, vec):
        self.pos[0] += vec[0]
        self.pos[1] += vec[1]
        for c in self.cards:
            p = c.getPos ()
            imagepos = c.getRect ()
            p = [p[0]+vec[0], p[1]+vec[1]]
            imagepos.left = p[0]
            imagepos.top  = p[1]
            c.putPos (p)
            surface.blit (c.getImage (), imagepos)

    def _down (self, surface):
        for c in self.cards:
            p = c.getPos ()
            imagepos = c.getRect ()
            imagepos.left = p[0]
            imagepos.top  = p[1]
            surface.blit (c.getImage (), imagepos)

    def isOn (self, pos, imagepos):
        return ((pos[0] >= imagepos.left) and (pos[0]-imagepos.left < imagepos.width) and
                (pos[1] >= imagepos.top) and (pos[1]-imagepos.top < imagepos.bottom))

    def isOnPile (self, pos):
        if not self.cardsUp:
            for c in self.cards:
                p = c.getPos ()
                imagepos = c.getRect ()
                imagepos.left = p[0]
                imagepos.top  = p[1]
                if self.isOn (pos, imagepos):
                    return True
        return False

    def redraw (self, surface):
        if not self.cardsUp:
            self._down (surface)

    def selectCard (self, pos):
        self.selected = -1
        if not self.cardsUp:
            l = self.cards  # remember l is not a copy, but a reference
            l.reverse ()
            for i, c in enumerate (l):
                p = c.getPos ()
                imagepos = c.getRect ()
                imagepos.left = p[0]
                imagepos.top  = p[1]
                if self.isOn (pos, imagepos):
                    l.reverse ()
                    self.selected = len(l)-i-1
                    print("selected card no", self.selected)
                    return
            l.reverse ()

    def flipSelectedCard (self, surface):
        if (not self.cardsUp) and (self.selected >= 0):
            self.cards[self.selected].flip ()
            for c in self.cards[self.selected:]:
                p = c.getPos ()
                imagepos = c.getRect ()
                imagepos.left = p[0]
                imagepos.top  = p[1]
                c.putPos (p)
                surface.blit (c.getImage (), imagepos)
            pygame.display.flip ()
            self.selected = -1

    def collect (self, surface, cloth, vec):
        if not self.cardsUp:
            self.pickUp (surface, cloth)
            self.collectVec = vec
            p = list (self.pos)
            for c in self.cards:
                imagepos = c.getRect ()
                imagepos.left = p[0]
                imagepos.top  = p[1]
                c.putPos (p)
                p[0] += vec[0]
                p[1] += vec[1]
            self.dropDown (surface)

    def split (self):
        print("selected =", self.selected)
        if (not self.cardsUp) and (self.selected >= 0) and (len (self.cards)>1):
            print("found selected card", self.selected)
            # p is the pile underneath self
            # l is the list of cards underneath self
            l = self.cards[:self.selected]
            p = cardPile (l,
                          list(l[0].getPos ()),
                          list(self.splay),
                          None)
            p.cardsUp = False
            self.cards = self.cards[self.selected:]
            # the list 'self' is positioned at the selected card
            self.pos = self.cards[0].getPos ()
            return p
        return None

    #
    # returns True if self can be dropped onto, p.
    #

    def dropPile (self, p):
        if (p != []) and (self.cards != []):
            for cj in p.cards:
                for ci in self.cards:
                    if cj.isOn (ci):
                        print("pile is on top of another")
                        return True
        return False


#
#  cardGroup - allows the programmer to give the user the ability to stack
#              cards and organise stacks of cards.
#

class cardGroup:

    def __init__ (self, cards=[], pos=[0, 0], splay=[SplayDrop, SplayDrop]):
        self.cards = cards
        self.pos = pos
        self.splay = splay
        self.piles = [cardPile (cards, pos, splay)]
        self.selected = -1

    def gather (self, surface, cloth):
        self.pickUp (surface, cloth)
        self.piles = [cardPile (self.cards, self.pos, self.splay)]
        self.selected = -1
        for c in self.cards:
            c.putPos (self.pos)
        self.dropDown (surface)

    def shuffle (self):
        for p in self.piles:
            p.shuffle ()

    def dropDown (self, surface):
        for p in self.piles:
            p.dropDown (surface)

    def pickUp (self, surface, cloth):
        for p in self.piles:
            p.pickUp (surface, cloth)

    def _pickUp (self, surface, cloth):
        for p in self.piles:
            p._pickUp (surface, cloth)

    def selectPile (self, pos):
        self.selected = -1
        for i, p in enumerate (self.piles):
            if p.isOnPile (pos):
                self.selected = i

    def _haveMovement (self, vec):
        return (vec[0] != 0) or (vec[1] != 0)

    def redraw (self, surface):
        for p in self.piles:
            p.redraw (surface)

    def movePile (self, surface, cloth, vec, groups):
        if (self.selected != -1) and (self.selected < len (self.piles)) and self._haveMovement (vec):
            for g in groups:
                for p in g.piles:
                    p._pickUp (surface, cloth)
            for g in groups:
                for i, p in enumerate (g.piles):
                    if i != g.selected:
                        p._down (surface)
            self.piles[self.selected].move (surface, vec)
            return True
        return False

    def dropPile (self, surface, groups):
        if self.selected >= 0:
            print("dropping selected pile", self.selected)
            for j, g in enumerate (groups):
                print("  checking group", j, "piles [", end=' ')
                for i, p in enumerate (g.piles):
                    print(i, end=' ')
                    if g == self or i == self.selected:
                        print("x", end=' ')
                        pass
                    elif self.piles[self.selected].dropPile (p):
                        print("y]", end=' ')
                        print(" dropped..", len(self.piles))
                        self.piles.pop (self.selected)
                        print("popped..", len(self.piles))
                        self.selected = -1
                        return
                    print("]")

    def selectCard (self, pos):
        for p in self.piles:
            p.selectCard (pos)

    def flipSelectedCard (self, surface):
        for p in self.piles:
            p.flipSelectedCard (surface)

    def collectSelectedPile (self, surface, cloth, vec=[0, 0]):
        if (self.selected != -1) and (self.selected < len (self.piles)):
            self.piles[self.selected].collect (surface, cloth, vec)

    def splitSelectedPile (self):
        if (self.selected != -1) and (self.selected < len (self.piles)):
            print("splitting selected pile", self.selected)
            p = self.piles[self.selected].split ()
            if p != None:
                self.piles.append (p)

    def shuffleSelectedPile (self, surface, cloth):
        if (self.selected != -1) and (self.selected < len (self.piles)):
            self.piles[self.selected]._shuffle (surface, cloth)
