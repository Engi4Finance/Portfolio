import rsk
import numpy as np
import keyboard
import time
import threading
import math

# Créer des verrous pour synchroniser l'accès aux robots
lock_green1 = threading.Lock()
lock_green2 = threading.Lock()

# Fonction pour déplacer un robot
def move_robot(robot, dx, dy, dz):
    try:
        robot.goto((robot.position[0] + dx, robot.position[1] + dy, 3.14 + dz))
        dz = dz + 0.3
    except Exception as e:
        print("An error occurred while moving the robot:", e)

# Fonction pour faire kicker un robot
def kick_robot(robot):
    try:
        robot.kick()
    except Exception as e:
        print("An error occurred while kicking the robot:", e)

# Fonction pour calculer l'orientation du robot vers la balle
def orient_towards_ball(robot, ball):
    dx = ball.x - robot.position[0]
    dy = ball.y - robot.position[1]
    angle = math.atan2(dy, dx) # Calcul de l'angle vers la balle
    desired_angle = angle + math.pi # Ajout de 180 degrés pour orienter le robot vers la balle
    move_robot(robot, 0, 0, desired_angle)

# Délai d'attente pour permettre au simulateur de répondre
SIMULATION_DELAY = 0.05 # 50 millisecondes

# Fonction pour gérer les mouvements du robot green1
def manage_green1():
    while True:
        if keyboard.is_pressed('left'):
            with lock_green1:
                move_robot(client.green1, -0.2, 0, 0)
            print('You Pressed left!')
        elif keyboard.is_pressed('right'):
            with lock_green1:
                move_robot(client.green1, 0.2, 0, 0)
            print('You Pressed right!')
        elif keyboard.is_pressed('down'):
            with lock_green1:
                move_robot(client.green1, 0, -0.2, 0)
            print('You Pressed down!')
        elif keyboard.is_pressed('up'):
            with lock_green1:
                move_robot(client.green1, 0, 0.2, 0)
            print('You Pressed up!')
        elif keyboard.is_pressed('p'):
            with lock_green1:
                kick_robot(client.green1)
            print('You kicked!')
        time.sleep(SIMULATION_DELAY)

# Fonction pour gérer les mouvements du robot green2
def manage_green2():
    while True:
        if keyboard.is_pressed('q'):
            with lock_green2:
                move_robot(client.green2, -0.2, 0, 0)
            print('You Pressed left!')
        elif keyboard.is_pressed('d'):
            with lock_green2:
                move_robot(client.green2, 0.2, 0, 0)
            print('You Pressed right!')
        elif keyboard.is_pressed('s'):
            with lock_green2:
                move_robot(client.green2, 0, -0.2, 0)
            print('You Pressed down!')
        elif keyboard.is_pressed('z'):
            with lock_green2:
                move_robot(client.green2, 0, 0.2, 0)
            print('You Pressed up!')
        elif keyboard.is_pressed('f'):
            with lock_green2:
                kick_robot(client.green2)
            print('You kicked!')
        time.sleep(SIMULATION_DELAY)

# Créer une instance de la classe Client en dehors de la boucle principale
with rsk.Client() as client:
    # Créer les threads pour gérer les mouvements des robots green1 et green2
    thread_green1 = threading.Thread(target=manage_green1)
    thread_green2 = threading.Thread(target=manage_green2)
    
    # Démarrer les threads
    thread_green1.start()
    thread_green2.start()
    
    # Attendre la fin des threads
    thread_green1.join()
    thread_green2.join()