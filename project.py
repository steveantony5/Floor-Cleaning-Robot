from PyQt5 import QtWidgets, QtGui,QtCore
from console import Ui_Dialog #importing the generated python class from.ui
from PyQt5.QtWidgets import QTableWidgetItem
import sys, os
import socket
from twilio.rest import Client

HOST = '10.0.0.199'
PORT = 8005

#inheriting mywindow class from the class QDialog
class mywindow(QtWidgets.QDialog):
    def __init__(self):
        super(mywindow, self).__init__() #initiating the parent class
        self.ui = Ui_Dialog() 
        self.ui.setupUi(self) #calling the function generated in the .ui
        

        self.ui.Auto.clicked.connect(self.Auto_mode) 

        self.ui.Manual.clicked.connect(self.Manual_mode) 

        self.ui.up.clicked.connect(self.up_move)

        self.ui.down.clicked.connect(self.down_move)

        self.ui.left.clicked.connect(self.left_move)

        self.ui.right.clicked.connect(self.right_move) 

        self.ui.stop.clicked.connect(self.stop_move) 

        self._update_timer = QtCore.QTimer()
        self._update_timer.timeout.connect(self.update_display)
        self._update_timer.start(2000) # milliseconds





    def Auto_mode(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("Auto mode\n")
            s.sendall(b'auto')
            # # Your Account SID from twilio.com/console
            # account_sid = "AC4395720c534e1be2cae59ad58fcfb8d3"
            # # Your Auth Token from twilio.com/console
            # auth_token  = "69133ef1e8bdda9079022e898aacb519"

            # client = Client(account_sid, auth_token)

            # message = client.messages.create(
            #     to="+17209175161", 
            #     from_="+17205730808",
            #     body="Floor cleaning Robot: Auto mode turned on! Welcome back to your cleaner home")

            # print(message.sid)



    def Manual_mode(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("Manual_mode\n")
            s.sendall(b'manual')

    def up_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("up_move\n")
            s.sendall(b'up')

    def down_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("down_move\n")
            s.sendall(b'down')

    def left_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("left_move\n")
            s.sendall(b'left')

    def right_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("right_move\n")
            s.sendall(b'right')

    def stop_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("stop\n")
            s.sendall(b'stop')



    def update_display(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("Display update")
            


            s.sendall(b'display')

            lux_value = s.recv(10)
            lux_decoded = lux_value.decode('UTF-8')
            lux_string = str(lux_decoded)
            print("lux value ", lux_string)
            self.ui.lux.setText(lux_string)

            motion_value = s.recv(10)
            motion_decoded = motion_value.decode('UTF-8')
            motion_string = str(motion_decoded)
            print("distance value ", motion_string)
            self.ui.motion.setText(motion_string)
            
            water_value = s.recv(10)
            water_decoded = water_value.decode('UTF-8')
            water_string = str(water_decoded)
            print("water level ", water_string)
            self.ui.water.setText(water_string)

            mode_value = s.recv(10)
            mode_decoded = mode_value.decode('UTF-8')
            mode_string = str(mode_decoded)
            print("mode ", mode_string)
            self.ui.mode.setText(mode_string)

            deg_mode_value = s.recv(10)
            deg_mode_decoded = deg_mode_value.decode('UTF-8')
            deg_mode_string = str(deg_mode_decoded)
            print("deg mode ", deg_mode_string)
            self.ui.deg_mode.setText(deg_mode_string)

            tiva_value = s.recv(10)
            tiva_decoded = tiva_value.decode('UTF-8')
            tiva_string = str(tiva_decoded)
            print("tiva mode ", tiva_string)
            self.ui.tiva.setText(tiva_string)

            ultra_status = s.recv(10)
            ultra_status_decoded = ultra_status.decode('UTF-8')
            ultra_status_decoded_string = str(ultra_status_decoded)
            print("ultrasonic status ", ultra_status_decoded_string)
            self.ui.ultra_sensor.setText(ultra_status_decoded_string)

            lux_status = s.recv(10)
            lux_status_decoded = lux_status.decode('UTF-8')
            lux_status_decoded_string = str(lux_status_decoded)
            print("lux status ", lux_status_decoded_string)
            self.ui.lux_sensor.setText(lux_status_decoded_string)

            water_status = s.recv(10)
            water_status_decoded = water_status.decode('UTF-8')
            water_status_decoded_string = str(water_status_decoded)
            print("water status ", water_status_decoded_string)
            self.ui.water_sensor.setText(water_status_decoded_string)

            valve_status = s.recv(10)
            valve_status_decoded = valve_status.decode('UTF-8')
            valve_status_decoded_string = str(valve_status_decoded)
            print("valve status ", valve_status_decoded_string)
            self.ui.valve.setText(valve_status_decoded_string)
   

    
  
def main():
    

    app = QtWidgets.QApplication([])
    application = mywindow()
    application.show()
    app.exec() # to execute the application

    
if __name__ == '__main__':
    main()






