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


SIMULATION_DELAY = 0.06  # Délai d'attente pour permettre au simulateur de répondre
MOVE_SPEED = 0.4  # Vitesse de déplacement
TURN_SPEED = 0.9  # Vitesse de rotation


# Dictionnaire pour stocker les états des touches pour chaque robot
key_state = {
    'blue1': {
        'j': False, 'l': False, 'i': False, 'k': False, 'o': False, 'u': False, 'h': False
    },
    'blue2': {
        'q': False, 'd': False, 'z': False, 's': False, 'a': False, 'e': False, 'f': False
    }
}

# Fonction générique pour gérer les mouvements des robots
def manage_robot(robot, key_map):
    while True:
        moved = False
        if key_map['i'] or key_map['z']:  # le robot va vers l'avant
            move_robot(robot, MOVE_SPEED, 0, 0)
            moved = True
        elif key_map['k'] or key_map['s']:  # le robot va en arrière
            move_robot(robot, -MOVE_SPEED, 0, 0)
            moved = True
        elif key_map['l'] or key_map['d']:  # le robot va à droite
            move_robot(robot, 0, -MOVE_SPEED, 0)
            moved = True
        elif key_map['j'] or key_map['q']:  # le robot va à gauche
            move_robot(robot, 0, MOVE_SPEED, 0)
            moved = True
        elif key_map['o'] or key_map['e']:  # le robot tourne vers la droite
            move_robot(robot, 0, 0, -TURN_SPEED)
            moved = True
        elif key_map['u'] or key_map['a']:  # le robot tourne vers la gauche
            move_robot(robot, 0, 0, TURN_SPEED)
            moved = True
        else:
            stop_robot(robot)  # Arrêter le robot si aucune touche n'est pressée

        if key_map['h'] or key_map['f']:  # le robot tire
            kick_robot(robot)

        time.sleep(SIMULATION_DELAY)

# Fonction pour mettre à jour l'état des touches
def on_press(event):
    for robot, keys in key_state.items():
        if event.name in keys:
            keys[event.name] = True
            print(f"Touche pressée pour {robot} : {event.name}")

def on_release(event):
    for robot, keys in key_state.items():
        if event.name in keys:
            keys[event.name] = False
            print(f"Touche relâchée pour {robot} : {event.name}")

# Créer une instance de la classe Client en dehors de la boucle principale
with rsk.Client(host="10.31.24.246", key="") as client:
    # Enregistrer les fonctions de gestion des événements pour les touches
    for keys in key_state.values():
        for key in keys:
            keyboard.on_press_key(key, on_press)
            keyboard.on_release_key(key, on_release)

    # Créer les threads pour gérer les mouvements des robots
    thread_blue1 = threading.Thread(target=manage_robot, args=(client.blue1, key_state['blue1']))
    thread_blue2 = threading.Thread(target=manage_robot, args=(client.blue2, key_state['blue2']))

    # Démarrer les threads
    thread_blue1.start()
    thread_blue2.start()

    # Attendre la fin des threads
    thread_blue1.join()
    thread_blue2.join()
