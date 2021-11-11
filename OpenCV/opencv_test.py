import cv2, pygame
import numpy as np
from time import sleep

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


cap = cv2.VideoCapture(1)#Which webcam we're using - change to 0 for production
_, frame = cap.read()
rows, cols, _ = frame.shape

centre = int(cols/2)
x_mid = centre # Avoids a crash if no object is detected

def yPxToDist(x):
    return 0.0002*x**2 - 0.0019*x + 10.462

def xPxToDist(x, ydist):
    grad = 0.0014*ydist - 0.0016
    return grad*x
    #org grad = 0.034

class token:
    def __init__ (self, x,y,w,h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    def getDist(self):
        metre_w = 200
        metre_h = 50

        dist1 = w/metre_w
        dist2 = h/metre_h

        dist = (dist1 + dist2)/2
        return dist1, dist2

    def getLocation(self):

        x_ord = x + w/2 - cols/2
        y_ord = rows - y - h

        return x_ord, y_ord

while True:
    _, frame = cap.read()
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    #Reset x_mid to centre of the screen if no object detected
    x_mid = centre

    #Find Red Coloured Objects
    low_red = np.array([161, 155, 60])
    high_red = np.array([179, 255, 255])
    red_mask = cv2.inRange(hsv_frame, low_red, high_red)
    contours, _ = cv2.findContours(red_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=lambda x:cv2.contourArea(x), reverse=True)

    for n in range(1): #Maximum number of objects we want to find
        try:
            (x, y, w, h) = cv2.boundingRect(contours[n])
            tok = token(x,y,w,h)
            loc = tok.getLocation()

            #print(tok.getLocation()) #Print x,y dist in px
            #print(yPxToDist(tok.getLocation()[1])) #Print y dist in cm

            ydist = yPxToDist(loc[1])
            xdist = xPxToDist(loc[0], ydist)

            dist = [[xdist*10, ydist*10]]
            print(dist)

            #generate_map(dist)
            #pygame.display.flip()

            x_mid = int((2*x + w)/2)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0,255,0), 2)
            cv2.line(frame, (x_mid, 0), (x_mid, 480), (0,255,0),2)
        except:
            pass



    cv2.imshow("Frame", frame)
    #cv2.imshow("mask", red_mask) #show what the computer sees

    key = cv2.waitKey(1)

    if key == 27:
        break
    """
    #Output if the object is to the left or right of the centre
    if x_mid < centre:
        print("Go Left")
    elif x_mid > centre:
        print("Go Right")
    """

    sleep(2)

cap.release()
cv2.destroyAllWindows()
