import rsk
import numpy as np
import keyboard
import time
import threading

# Fonction pour déplacer un robot
def move_robot(robot, dx, dy, dz):
    try:
        robot.goto((robot.position[0] + dx, robot.position[1] + dy, 3.14 + dz))
        dz += 0.3
    except Exception as e:
        print("Une erreur s'est produite lors du déplacement du robot :", e)

# Fonction pour faire kicker un robot
def kick_robot(robot):
    try:
        robot.kick()
    except Exception as e:
        print("Une erreur s'est produite lors du kick du robot :", e)

# Variables pour suivre l'état des touches
keys_state = {
    'left': False,
    'right': False,
    'up': False,
    'down': False
}

# Vitesse de déplacement
MOVE_SPEED = 0.1

# Fonction pour gérer les mouvements du robot green1
def manage_green1():
    last_update_time = time.time()
    while True:
        current_time = time.time()
        delta_time = current_time - last_update_time
        
        dx = (keys_state['right'] - keys_state['left']) * MOVE_SPEED * delta_time
        dy = (keys_state['down'] - keys_state['up']) * MOVE_SPEED * delta_time
        
        move_robot(client.green1, dx, dy, 0)
        
        last_update_time = current_time
        time.sleep(0.01)

# Fonction pour détecter les touches pressées et relâchées
def on_press(event):
    key = event.name
    if key in keys_state:
        keys_state[key] = True

def on_release(event):
    key = event.name
    if key in keys_state:
        keys_state[key] = False

# Créer une instance de la classe Client en dehors de la boucle principale
with rsk.Client() as client:
    # Créer le thread pour gérer les mouvements du robot green1
    thread_green1 = threading.Thread(target=manage_green1)
    
    # Associer les fonctions aux événements de pression et de relâchement des touches
    keyboard.on_press(on_press)
    keyboard.on_release(on_release)
    
    # Démarrer le thread
    thread_green1.start()
    
    # Attendre la fin du thread
    thread_green1.join()
