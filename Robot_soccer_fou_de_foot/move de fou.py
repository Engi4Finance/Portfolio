import rsk
import numpy as np
import math


with rsk.Client() as client:
    (Xb , Yb) = client.ball
    (X1 , Y1) = client.green1.position
    (X2 , Y2) = client.green2.position
    D1 = math.sqrt((Xb-X1)**2+ (Yb-Y1)**2)  
    D2 = math.sqrt((Xb-X2)**2 + (Yb-Y2)**2)      
    D = min(D1,D2)
    while Xb >= -0.7  :    
        arrived = False
        while not arrived:
            if D1 <= D2 and X1 > Xb:
                robot_1_arrived = client.green1.goto((Xb + 0.08, Yb, 3.14), wait = False)  # On demande au robot d'aller en (0,0) avec une orientation de 0 radian                
                robot_2_arrived = client.green2.goto((0.9, Yb, 3.14),wait = False)
                client.green1.kick ()
            elif D1 > D2 and X2 > Xb:
                robot_2_arrived = client.green2.goto((Xb + 0.08, Yb, 3.14), wait = False)  # On demande au robot d'aller en (0,0) avec une orientation de 0 radian
                robot_1_arrived = client.green1.goto((0.9, Yb, 3.14),wait = False)
                client.green2.kick ()
            elif D1 < D2 and X1 < Xb:
                robot_2_arrived = client.green2.goto((Xb + 0.08, Yb, 3.14), wait = False)  # On demande au robot d'aller en (0,0) avec une orientation de 0 radian
                robot_1_arrived = client.green1.goto((0.9, Yb, 3.14),wait = False)
                client.green2.kick ()
            elif D1 >= D2 and X2 < Xb:
                robot_1_arrived = client.green1.goto((Xb + 0.08, Yb, 3.14), wait = False)  # On demande au robot d'aller en (0,0) avec une orientation de 0 radian
                robot_2_arrived = client.green2.goto((0.9, Yb, 3.14),wait = False)
                client.green1.kick ()
            arrived = robot_1_arrived and robot_2_arrived
    while Xb < -0.75:
        arrived = False
        while not arrived:
            if D1 < D2:
                robot_1_arrived = client.green1.goto((-0.5, Yb, 3.14), wait = False)  # On demande au robot d'aller en (0,0) avec une orientation de 0 radian
                robot_2_arrived = client.green2.goto((0.9, 0, 3.14),wait = False)      
            else :
                robot_2_arrived = client.green2.goto((-0.5, Yb, 3.14), wait = False)  # On demande au robot d'aller en (0,0) avec une orientation de 0 radian
                robot_1_arrived = client.green1.goto((0.9, 0, 3.14),wait = False)      
            arrived = robot_1_arrived and robot_2_arrived


# orientation sans intégration au robot

import rsk
import numpy as np
import math
import time


def orientation(R):
   
   while True:


        with rsk.Client() as client:


            xc = -0.9 # coordonee du centre cage
            yc = 0
       
            (Xb , Yb) = client.ball


           
               
            D3= xc - Xb # distance x horizontal entre ballon et le centre cage


            D4 = yc - Yb # distance y vertical entre ballon et le centre cage


            R = math.atan(D4 / D3)  # angle du balon par rapport a la ligne de touche


            #print (D3)
            #print (D4)
            #print (R*180/3.1415)
            time.sleep(1)

