#!/usr/bin/env python3

import os, pygame

dataDirectory = ".."
soundDict = {}
initialised = False


def getFileName (subdir, filename):
    global dataDirectory

    if dataDirectory == "":
        return os.path.join(subdir, filename)
    else:
        return os.path.join(dataDirectory, os.path.join(subdir, filename))


def load_sound (name):
    class NoneSound:
        def play(self):
            pass
    if not pygame.mixer or not pygame.mixer.get_init():
        return NoneSound()
    fullname = getFileName ('sounds', name)
    try:
        sound = pygame.mixer.Sound(fullname)
    except pygame.error as message:
        print(('cannot load sound file:', fullname))
        return NoneSound()
    return sound


def addSound (name):
    global soundDict
    soundDict[name] = load_sound(name + ".wav")


def initSounds ():
    addSound ("shuffle")
    addSound ("cardsnap")
    addSound ("start")
    addSound ("applause")
    addSound ("exit")


def playSound (sound):
    global soundDict

    init ()
    if sound in soundDict:
       soundDict[sound].play()
    else:
       print(("no sound file ", sound, " found"))


def init ():
    global initialised

    if not initialised:
        initSounds ()
        initialised = True
