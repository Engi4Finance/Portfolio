import rsk
import keyboard
import time


# Fonction pour déplacer un robot en utilisant la méthode control
def move_robot(robot, dx, dy, dturn):
    try:
        robot.control(dx, dy, dturn)
    except Exception as e:
        print("Une erreur s'est produite lors du déplacement du robot:", e)

# Fonction pour faire kicker un robot
def kick_robot(robot):
    try:
        robot.kick()
    except Exception as e:
        print("Une erreur s'est produite lors du kick du robot:", e)


SIMULATION_DELAY = 0.06

# Vitesse de déplacement
MOVE_SPEED = 0.2  # Ajustez selon vos besoins
TURN_SPEED = 0.9  # Vitesse de rotation


def manage_robots(client):
    while True:
        moved_green1 = False
        moved_green2 = False

        # Contrôle pour green1
        if keyboard.is_pressed('i'):  # le robot va vers l'avant
            move_robot(client.green1, MOVE_SPEED, 0, 0)
            moved_green1 = True
        elif keyboard.is_pressed('k'):  # le robot va en arrière
            move_robot(client.green1, -MOVE_SPEED, 0, 0)
            moved_green1 = True
        elif keyboard.is_pressed('l'):  # le robot va à droite
            move_robot(client.green1, 0, -MOVE_SPEED, 0)
            moved_green1 = True
        elif keyboard.is_pressed('j'):  # le robot va à gauche
            move_robot(client.green1, 0, MOVE_SPEED, 0)
            moved_green1 = True
        elif keyboard.is_pressed('o'):  # le robot tourne vers la droite
            move_robot(client.green1, 0, 0, -TURN_SPEED)
            moved_green1 = True
        elif keyboard.is_pressed('u'):  # le robot tourne vers la gauche
            move_robot(client.green1, 0, 0, TURN_SPEED)
            moved_green1 = True

        if not moved_green1:
            move_robot(client.green1, 0, 0, 0)  # Arrêter le robot si aucune touche n'est pressée

        if keyboard.is_pressed('h'):
            kick_robot(client.green1)

        # Contrôle pour green2
        if keyboard.is_pressed('z'):  # le robot va vers l'avant
            move_robot(client.green2, MOVE_SPEED, 0, 0)
            moved_green2 = True
        elif keyboard.is_pressed('s'):  # le robot va en arrière
            move_robot(client.green2, -MOVE_SPEED, 0, 0)
            moved_green2 = True
        elif keyboard.is_pressed('d'):  # le robot va à droite
            move_robot(client.green2, 0, -MOVE_SPEED, 0)
            moved_green2 = True
        elif keyboard.is_pressed('q'):  # le robot va à gauche
            move_robot(client.green2, 0, MOVE_SPEED, 0)
            moved_green2 = True
        elif keyboard.is_pressed('e'):  # le robot tourne vers la droite
            move_robot(client.green2, 0, 0, -TURN_SPEED)
            moved_green2 = True
        elif keyboard.is_pressed('a'):  # le robot tourne vers la gauche
            move_robot(client.green2, 0, 0, TURN_SPEED)
            moved_green2 = True

        if not moved_green2:
            move_robot(client.green2, 0, 0, 0)  # Arrêter le robot si aucune touche n'est pressée

        if keyboard.is_pressed('f'):  # le robot tire
            kick_robot(client.green2)

        time.sleep(SIMULATION_DELAY)


with rsk.Client(host="", key="") as client:
    manage_robots(client)
