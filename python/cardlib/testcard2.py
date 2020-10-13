#/usr/bin/env python3

import os, pygame, math, time, sys
from pygame.locals import *

from cardImages import cardImages
from cardGroup import cardGroup
from card import card
from cardEvent import cardEvent
import random

GreenTable = (0x19, 0x50, 0x32)
SplayDrop  = 10
Debugging = True


def main ():
    global screen
    pygame.init ()
    screen = pygame.display.set_mode((900, 768), HWSURFACE|RESIZABLE)
    screen.fill (GreenTable)
    pygame.display.set_caption ('Cluedo - v1.0')
    pygame.display.flip ()

    print("loading card images: ", end=' ')
    ci = cardImages ()
    print("done")

    pg = cardGroup (ci.getPlayers (), [10, 10], [10, 10])
    pg.shuffle ()
    pg.dropDown (screen)

    if Debugging:
        ce = cardEvent (screen, GreenTable, [pg])
    else:
        rg = cardGroup (ci.getRooms (), [300, 10], [10, 10])
        rg.shuffle ()
        rg.dropDown (screen)

        wg = cardGroup (ci.getWeapons (), [600, 10], [10, 10])
        wg.shuffle ()
        wg.dropDown (screen)

        eg = cardGroup ([ci.getEnvelope ()], [300, 400], [10, 10])
        eg.dropDown (screen)

        pi = cardGroup (ci.getPlayerIcons (), [600, 400], [10, 10])
        pi.dropDown (screen)

        ce = cardEvent (screen, GreenTable, [pg, rg, wg, eg, pi])

    ce.mainLoop ()

main()
