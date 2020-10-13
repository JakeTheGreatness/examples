#!/usr/bin/env python3

import os, pygame, math
from pygame.locals import *
from card import card, load_image


class cardImages:
    complete_card_list = []
    back = None
    envelope = None
    people = []
    room = []
    weapon = []
    peopleIcon = []

    def __init__ (self):
        self.back = load_image("back.png")
        self.envelope = card("envelope.png", load_image("envelope.png"))

        for p in ['scarlet', 'green', 'mustard', 'plum', 'white', 'peacock']:
            self.people.append(card("%s.png" % (p), self.back))

        for r in ['ballroom', 'conservatory', 'kitchen', 'lounge', 'billiardroom', 'diningroom', 'hall', 'library', 'study']:
            self.room.append(card("%s.png" % (r), self.back))

        for w in ['dagger', 'leadpiping', 'revolver', 'spanner', 'candlestick', 'rope']:
            self.weapon.append(card("%s.png" % (w), self.back))

        for i in ['green-icon', 'plum-icon', 'scarlet-icon', 'white-icon', 'mustard-icon', 'peacock-icon']:
            self.peopleIcon.append(card("%s.png" % (i), load_image("%s.png" % (i))))

    def getPlayer (self, n):
        return self.people[n]

    def getWeapon (self, n):
        return self.weapon[n]

    def getRoom (self, n):
        return self.room[n]

    def getBack(self):
        return self.back

    def getPlayers (self):
        return self.people

    def getWeapons (self):
        return self.weapon

    def getRooms (self):
        return self.room

    def getEnvelope (self):
        return self.envelope

    def getPlayerIcons (self):
        return self.peopleIcon

    def getPlayerIcon (self, n):
        return self.peopleIcon[n]
