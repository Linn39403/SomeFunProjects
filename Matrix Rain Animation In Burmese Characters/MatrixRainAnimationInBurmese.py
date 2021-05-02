import random as rd
import pygame, time
from datetime import datetime

burmeseCharacters = [chr(i) for i in range(0x1000, 0x102A)]
burmeseCharacters.extend([chr(i) for i in range(0x1040, 0x1049)])

white = (255, 255, 255)
brightgrey =(200,200,200)#(235,236,240)
black = (0, 0, 0)
neon = (57, 255, 100,0)

windowWidth = 1200
windowHeight = 800
display_surface = pygame.display.set_mode((windowWidth, windowHeight))
charString = []
stringLength = (5, 20)  # String Length min , max
verticalSpeed = (1, 10) # Veritcally scrolling speed range
stringMoveDelay = 20
regenerateStringDelay = 1000
gameEventTime = 0
gameEventTime2 = 0
stringCount = int(windowWidth / 20) - 1
stringSpeed = []

rd.seed(datetime.now())
pygame.init()
pygame.display.set_caption('Matrix Rain Code Animation in Burmese')
font = pygame.font.SysFont("myanmartext", 10)
clock = pygame.time.Clock()

for i in range(0,stringCount):
    stringSpeed.append( rd.randint(verticalSpeed[0], verticalSpeed[1]) )

def getRandomCharacter():
    return (rd.choice(burmeseCharacters))

def createCharString(xLoc,spd):
    y = 0
    idx = 0
    local_string = []
    #The random speed effect the each string
    #vy = rd.randint(verticalSpeed[0], verticalSpeed[1])
    for i in range(0, rd.randint(stringLength[0], stringLength[1])):
        y -= 14
        local_string.append({"x": xLoc,
                             "y": y,
                             "idx":idx,
                             "vy": spd,
                             "ch": getRandomCharacter()})
        idx+=1
    return local_string

def createMultipleString():
   global charString
   for x in range(0,stringCount):
       #We will append a new string only after the string last index is in the visible range
       lastIndex = len(charString[x])-1
       if( charString[x][lastIndex]["y"] > 10):
           charString[x].extend(createCharString((x+1)*20,stringSpeed[x]))

def moveCharString():
    # Move the string vertically and randomly change each char
    global charString
    for eachString in charString:
      for string in eachString:
        string["ch"] = getRandomCharacter()
        string["y"] += string["vy"]

def displayCharString():
    global charString, display_surface
    for eachString in charString:
      # Change the first index value color to grey
      for string in eachString:
        if(string["idx"] == 0 ):
          text = font.render(string["ch"], False, brightgrey, black)
        else:
          text = font.render(string["ch"], False, neon, black)
        textRect = text.get_rect()
        textRect.center = (string["x"], string["y"])
        text.set_alpha(255-string["idx"]*8)
        display_surface.blit(text, textRect)


def checkBountary():
  global charString, display_surface
  for eachString in charString:
    for string in eachString[:]:
      if(string["y"] > windowHeight):
        eachString.remove(string)

for x in range(0,stringCount):
    charString.append(createCharString((x+1)*14,stringSpeed[x]))

while True:
    display_surface.fill(black)

    if (pygame.time.get_ticks() - gameEventTime > stringMoveDelay):
        gameEventTime = pygame.time.get_ticks()
        moveCharString()
    if(pygame.time.get_ticks() - gameEventTime2 > regenerateStringDelay):
        gameEventTime2 = pygame.time.get_ticks()
        createMultipleString()

    displayCharString()
    checkBountary()

    # iterate over the list of Event objects
    # that was returned by pygame.event.get() method.
    for event in pygame.event.get():
        # if event object type is QUIT
        # then quitting the pygame
        # and program both.
        if event.type == pygame.QUIT:
            # deactivates the pygame library
            pygame.quit()

            # quit the program.
            quit()

    clock.tick(60)
    pygame.display.update()