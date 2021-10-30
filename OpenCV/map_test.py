import pygame
from time import sleep

pygame.init()

width = 1000
height = 500

surface = pygame.display.set_mode((width,height))# 1px is 3mm

token_colour = (255,0,0)
base_colour = (0,0,255)

base_dimensions = 350/3
token_dimensions = 31.8/3

def draw_base(x,y):
    pygame.draw.rect(surface, base_colour, pygame.Rect(x,y,base_dimensions, base_dimensions))

def draw_token(x,y):
    pygame.draw.rect(surface, token_colour, pygame.Rect(x, y, token_dimensions,token_dimensions ))

draw_base(0,(height-base_dimensions)/2)
draw_base(width-base_dimensions,(height-base_dimensions)/2)

#Draw the fixed tokens
draw_token((width+token_dimensions)/2, (height+token_dimensions)/2)
draw_token((width+token_dimensions)/2 -100, (height+token_dimensions)/2-100)
draw_token((width+token_dimensions)/2 +100, (height+token_dimensions)/2+100)

def draw_unknown_tokens(coordinates):
    for ords in coordinates:
        draw_token(ords[0]-token_dimensions/2, ords[1]-token_dimensions/2)

test_token_positions=[ [100, 20], [50,78]]

draw_unknown_tokens(test_token_positions)

pygame.display.flip()

sleep(100)
