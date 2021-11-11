import pygame
from time import sleep

test_token_positions=[ [50, 270] ]

def draw_base(x,y, surface, base_dimensions = 350/3):
    pygame.draw.rect(surface, (0,0,255), pygame.Rect(x,y,base_dimensions, base_dimensions))

def draw_token(x,y, surface, token_dimensions=31.8/3):
    pygame.draw.rect(surface, (255,0,0), pygame.Rect(x, y, token_dimensions,token_dimensions ))

def draw_unknown_tokens(coordinates, surface, token_dimensions=31.8/3):
    for ords in coordinates:
        draw_token(ords[0]-token_dimensions/2, ords[1]-token_dimensions/2, surface)

def generate_map(coordinates):

    pygame.init()

    width = 1000
    height = 500

    surface = pygame.display.set_mode((width,height))# 1px is 3mm

    base_dimensions = 350/3
    token_dimensions = 31.8/3

    #Draw the two bases
    draw_base(0,(height-base_dimensions)/2, surface)
    draw_base(width-base_dimensions,(height-base_dimensions)/2, surface)

    #Draw the fixed tokens
    draw_token((width+token_dimensions)/2, (height+token_dimensions)/2, surface)
    draw_token((width+token_dimensions)/2 -100, (height+token_dimensions)/2-100, surface)
    draw_token((width+token_dimensions)/2 +100, (height+token_dimensions)/2+100, surface)

    draw_unknown_tokens(test_token_positions, surface)

if __name__=="__main__":

    generate_map(test_token_positions)

    pygame.display.flip() #Display the map

    sleep(100)

    #Need to implement a saving feature
