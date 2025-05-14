import rsk
import numpy as np
import math
import time

cages = (0.27,0,-2.27)
Xb=Yb=X1g=Y1g=X2g=Y2g=X1b=Y1b=X2b=Y2b=0
distance_tir=0.02
y_cage_haut=0.24771468144044323
y_cage_bas=-0.36987534626038787
x_cage_bas=-0.9175298804780876

x_centre = 0
y_centre = 0
rayon = 0.3
n_points = 20

y_cage_bas_proche=-0.29891067538126365
y_cage_haut_proche=0.29891067538126365

def position_robot():
    global Xb,Yb,X1g,Y1g,X2g,Y2g,X1b,Y1b,X2b,Y2b
    (Xb , Yb) = client.ball
    (X1g , Y1g) = client.green1.position
    (X2g , Y2g) = client.green2.position
    (X1b , Y1b) = client.blue1.position
    #(X2b , Y2b) = client.blue2.position
    return Xb,Yb,X1g,Y1g,X2g,Y2g,X1b,Y1b,X2b,Y2b

def generer_points_cercle(x_centre, y_centre, rayon, n_points):
    points = []
    for i in range(n_points):
        angle = 2 * math.pi * i / n_points  # Divise le cercle en n_points segments
        x = x_centre + rayon * math.cos(angle)
        y = y_centre + rayon * math.sin(angle)
        points.append((x, y))
        client.green1.goto((x,y,3.14))
    client.green1.goto((0.3,0,3.14))
    return points

with rsk.Client() as client:
    position_robot()
    points = generer_points_cercle(x_centre, y_centre, rayon, n_points)
