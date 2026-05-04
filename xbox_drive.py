import pygame
import socket
import time
import sys

# ==========================================
# 1. CONFIGURATION
# ==========================================
CAR_IP = "10.152.1.186"  
CAR_PORT = 1221         

# ==========================================
# 2. NETWORK SETUP
# ==========================================
print(f"Connecting to your MSP430 Car at {CAR_IP}:{CAR_PORT}...")
car_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
car_socket.settimeout(3.0) 

try:
    car_socket.connect((CAR_IP, CAR_PORT))
    print("SUCCESS: Link Established! \n")
except Exception as e:
    print(f"FAILED TO CONNECT: {e}")
    sys.exit()

# ==========================================
# 3. XBOX CONTROLLER SETUP
# ==========================================
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No Xbox controller detected! Please connect it via Bluetooth or USB.")
    sys.exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Controller Connected: {joystick.get_name()}")

def send_command(cmd_string):
    try:
        car_socket.sendall(cmd_string.encode())
    except Exception as e:
        pass

# ==========================================
# 4. ROCKET LEAGUE DRIVING LOOP
# ==========================================
print("\n--- ROCKET LEAGUE MODE ---")
print("GAS: Right Trigger (RT)")
print("REVERSE: Left Trigger (LT)")
print("STEERING: D-Pad")
print("EMERGENCY STOP & QUIT: 'B' Button\n")

running = True
current_state = "STOP"

while running:
    # Clear the event queue so pygame reads the controller cleanly
    pygame.event.pump()
    
    # 1. Read D-Pad (Returns -1, 0, or 1 for X and Y)
    hat_x, hat_y = joystick.get_hat(0)
    
    # 2. Read Triggers (On Mac Xbox controllers, LT is usually Axis 4, RT is Axis 5)
    # Triggers rest at -1.0 and go to 1.0 when pulled. We check if they are > 0 (pulled halfway).
    try:
        lt_pulled = joystick.get_axis(4) > 0.0
        rt_pulled = joystick.get_axis(5) > 0.0
    except:
        lt_pulled = False
        rt_pulled = False

    new_state = "STOP"

    # PRIORITY 1: STEERING (Overrides Gas/Brake for quick pivots)
    if hat_x == -1:
        new_state = "LEFT"
    elif hat_x == 1:
        new_state = "RIGHT"
        
    # PRIORITY 2: GAS & BRAKE (Triggers or Up/Down on D-Pad as a backup)
    elif rt_pulled or hat_y == 1:
        new_state = "FORWARD"
    elif lt_pulled or hat_y == -1:
        new_state = "REVERSE"

    # SEND COMMAND ONLY IF THE CAR NEEDS TO CHANGE WHAT IT IS DOING
    if new_state != current_state:
        if new_state == "FORWARD":
            send_command("^1221F999")
        elif new_state == "REVERSE":
            send_command("^1221B999")
        elif new_state == "LEFT":
            send_command("^1221L999")
        elif new_state == "RIGHT":
            send_command("^1221R999")
        elif new_state == "STOP":
            send_command("^1221W000")
        
        print(f"Action: {new_state}")
        current_state = new_state

    # Check if you pressed 'B' to quit
    for event in pygame.event.get():
        if event.type == pygame.JOYBUTTONDOWN:
            if joystick.get_button(1): # 'B' Button
                print("\nEmergency Stop! Disconnecting...")
                send_command("^1221W000")
                running = False

    # Tiny delay to prevent overloading the laptop CPU
    time.sleep(0.01) 

# Clean up
car_socket.close()
pygame.quit()