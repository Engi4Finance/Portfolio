import rsk
import keyboard
import time
import threading


# Fonction pour déplacer un robot en utilisant la méthode control

def move_robot(robot, dx, dy, dturn):
    try:
        robot.control(dx, dy, dturn)
    except Exception as e:
        print("Une erreur s'est produite lors du déplacement du robot:", e)


# Fonction pour arrêter le mouvement d'un robot

def stop_robot(robot):
    try:
        robot.control(0, 0, 0)
    except Exception as e:
        print("Une erreur s'est produite lors de l'arrêt du robot:", e)


# Fonction pour faire kicker un robot
def kick_robot(robot):
    try:
        robot.kick()
    except Exception as e:
        print("Une erreur s'est produite lors du kick du robot:", e)




SIMULATION_DELAY = 0.06 # Délai d'attente pour permettre au simulateur de répondre
MOVE_SPEED = 0.4  # Vitesse de déplacement
TURN_SPEED = 0.9  # Vitesse de rotation

# Dictionnaires pour stocker les états des touches pour chaque robot
key_state_green1 = {
    'j': False,
    'l': False,
    'i': False,
    'k': False,
    'o': False,
    'u': False,
    'h': False
}
key_state_green2 = {
    'q': False,
    'd': False,
    'z': False,
    's': False,
    'a': False,
    'e': False,
    'f': False
}

def manage_green1():
    while True:
        moved = False
        if key_state_green1['i']:  # le robot va vers l'avant
            move_robot(client.green1, MOVE_SPEED, 0, 0) 
            moved = True
        elif key_state_green1['k']:# le robot va en arrière
            move_robot(client.green1, -MOVE_SPEED, 0, 0)
            moved = True
        elif key_state_green1['l']:  # le robot va à droite
            move_robot(client.green1, 0, -MOVE_SPEED, 0)
            moved = True
        elif key_state_green1['j']: # le robot va à gauche
            move_robot(client.green1, 0, MOVE_SPEED, 0)
            moved = True
        elif key_state_green1['o']:  # le robot tourne vers la droite
            move_robot(client.green1, 0, 0, -TURN_SPEED)
            moved = True
        elif key_state_green1['u']:  # le robot tourne vers la gauche
            move_robot(client.green1, 0, 0, TURN_SPEED)
            moved = True

        if not moved:
            stop_robot(client.green1)  # Arrêter le robot si aucune touche n'est pressée

        if key_state_green1['h']:
            kick_robot(client.green1)

        time.sleep(SIMULATION_DELAY)

def manage_green2():
    while True:
        moved = False
        if key_state_green2['z']:  # le robot va vers l'avant
            move_robot(client.green2, MOVE_SPEED, 0, 0)
            moved = True
        elif key_state_green2['s']: # le robot va en arrière
            move_robot(client.green2, -MOVE_SPEED, 0, 0)
            moved = True
        elif key_state_green2['d']: # le robot va à droite
            move_robot(client.green2, 0, -MOVE_SPEED, 0)
            moved = True
        elif key_state_green2['q']: # le robot va à gauche
            move_robot(client.green2, 0, MOVE_SPEED, 0)
            moved = True
        elif key_state_green2['e']: # le robot tourne vers la droite
            move_robot(client.green2, 0, 0, -TURN_SPEED)
            moved = True
        elif key_state_green2['a']: # le robot tourne vers la gauche
            move_robot(client.green2, 0, 0, TURN_SPEED)
            moved = True
        if not moved:
            stop_robot(client.green2)  # Arrêter le robot si aucune touche n'est pressée

        if key_state_green2['f']: # le robot tire
            kick_robot(client.green2)

        time.sleep(SIMULATION_DELAY)

# Fonctions pour mettre à jour l'état des touches pour green1
def on_press_green1(event):
    if event.name in key_state_green1:
        key_state_green1[event.name] = True
        print(f"Touche pressée pour green1 : {event.name}")

def on_release_green1(event):
    if event.name in key_state_green1:
        key_state_green1[event.name] = False
        print(f"Touche relâchée pour green1 : {event.name}")

# Fonctions pour mettre à jour l'état des touches pour green2
def on_press_green2(event):
    if event.name in key_state_green2:
        key_state_green2[event.name] = True
        print(f"Touche pressée pour green2 : {event.name}")

def on_release_green2(event):
    if event.name in key_state_green2:
        key_state_green2[event.name] = False
        print(f"Touche relâchée pour green2 : {event.name}")

# Créer une instance de la classe Client en dehors de la boucle principale
with rsk.Client(host="", key="") as client:

    
    # Enregistrer les fonctions de gestion des événements pour green1
    for key in key_state_green1.keys():
        keyboard.on_press_key(key, on_press_green1)
        keyboard.on_release_key(key, on_release_green1)

    # Enregistrer les fonctions de gestion des événements pour green2
    for key in key_state_green2.keys():
        keyboard.on_press_key(key, on_press_green2)
        keyboard.on_release_key(key, on_release_green2)

    # Créer les threads pour gérer les mouvements des robots green1 et green2
    thread_green1 = threading.Thread(target=manage_green1)
    thread_green2 = threading.Thread(target=manage_green2)

    # Démarrer les threads
    thread_green1.start()
    thread_green2.start()

    # Attendre la fin des threads
    thread_green1.join()
    thread_green2.join()
