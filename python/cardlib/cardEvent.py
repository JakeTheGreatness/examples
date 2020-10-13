#!/usr/bin/env python3

import os, pygame, math
from pygame.locals import *


class cardEvent:
    def __init__ (self, surface, cloth, groups):
        self.groups = groups
        self.surface = surface
        self.cloth = cloth
        self.groups = groups

    def groupsDown (self):
        for g in self.groups:
            g.dropDown (self.surface)

    def _selectPile (self, pos):
        for g in self.groups:
            g.selectPile (pos)

    def _selectCard (self, pos):
        for g in self.groups:
            g.selectCard (pos)

    def _flipCard (self, surface):
        for g in self.groups:
            g.flipSelectedCard (surface)

    def _moveSelected (self, pos):
        changed = False
        for g in self.groups:
            changed = g.movePile (self.surface, self.cloth, pos, self.groups) or changed
        if changed:
            pygame.display.flip()

    def redraw (self):
        self.surface.fill (self.cloth)
        for g in self.groups:
            g.redraw (self.surface)

    def _doReSize (self, event):
        self.surface = pygame.display.set_mode (event.dict['size'], HWSURFACE|DOUBLEBUF|RESIZABLE)
        self.redraw ()
        pygame.display.flip()

    def _collectSelectedPile (self, vec=[0, 0]):
        for g in self.groups:
            g.collectSelectedPile (self.surface, self.cloth, vec)

    def _splitPile (self):
        for g in self.groups:
            g.splitSelectedPile ()

    def shuffleSelectedPile (self):
        for g in self.groups:
            g.shuffleSelectedPile (self.surface, self.cloth)

    def doSplit (self):
        # self._selectPile (pygame.mouse.get_pos ())
        self._selectPile (pygame.mouse.get_pos ())
        self._selectCard (pygame.mouse.get_pos ())
        x = pygame.mouse.get_rel ()  # ignore first call to get_rel
        self._splitPile ()

    def doSelect (self):
        # left button down attempts to select a pile
        self._selectPile (pygame.mouse.get_pos ())
        self._selectCard (pygame.mouse.get_pos ())
        x = pygame.mouse.get_rel ()  # ignore first call to get_rel

    def doFlip (self):
        # right button flips card over
        self._selectCard (pygame.mouse.get_pos ())
        self._flipCard (self.surface)

    def gatherPiles (self):
        for g in self.groups:
            g.gather (self.surface, self.cloth)

    def dropPile (self):
        for g in self.groups:
            g.dropPile (self.surface, self.groups)

    def mainLoop (self):
        handleMotion = False
        while True:
            for event in pygame.event.get():
                if event.type == KEYDOWN:
                    if event.key == K_ESCAPE:
                        return
                    elif event.key == K_F1:
                        # collect selected group
                        self._collectSelectedPile ()
                    elif event.key == K_F2:
                        self._collectSelectedPile ([0, 40])
                    elif event.key == K_F3:
                        self._collectSelectedPile ([60, 0])
                    elif event.key == K_F4:
                        self._collectSelectedPile ([40, 50])
                    elif event.key == K_F5:
                        self.shuffleSelectedPile ()
                    elif event.key == K_F12:
                        self.gatherPiles ()

                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button in [1, 2, 3]:
                        if event.button == 2:
                            self.doSplit ()
                            handleMotion = True
                        if event.button == 1:
                            self.doSelect ()
                            handleMotion = True
                        if event.button == 3:
                            self.doFlip ()

                elif event.type == pygame.MOUSEBUTTONUP:
                    if event.button in [1]:
                        # left button up, drops selected pile
                        handleMotion = False
                        self.dropPile ()
                    elif event.button in [2]:
                        handleMotion = False
                        self.dropPile ()

                elif handleMotion and (event.type == pygame.MOUSEMOTION):
                    self._moveSelected (pygame.mouse.get_rel ())

                elif event.type == VIDEORESIZE:
                    self._doReSize (event)
