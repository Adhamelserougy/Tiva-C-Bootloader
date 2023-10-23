import tkinter as tk
import PySimpleGUI as sg
import socket

# for dark theme
sg.theme("DarkAmber") 

# This function opens file from specific path and sends it to over specific socket


def send_over_uart(file_path):
    # open file from specific path
    with open(file_path, "r") as f:
        lines = f.readlines()

    output = ""
    for line in lines:
        if line.startswith("@"):  # if line starts with @ character skip it
            continue
        if line.startswith("q"):  # if line starts with q character skip it
            continue
        new_line = line.split()
        output += "".join(new_line)
    ##        if len(new_line) % 16 != 0: # if line not o(16) complete it with zeros until 16
    ##            output += '0' * (16 - len(output) % 16)

    byte_output = bytearray.fromhex(output)  # convert file array to hex

    print(len(byte_output))  # print length on terminal
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    HOST = "192.168.4.1"
    PORT = 10000
    server.connect((HOST, PORT))
    server.sendall(byte_output,socket.MSG_WAITALL)
    message = server.recv(len(byte_output))
    server.close()


column_to_be_centered = [[sg.Image("IC3.PNG", size=(250, 250))]]

# for GUI
layout = [
    [
        sg.Image(
            "IC3.PNG",
            size=(250, 250),
            pad=((10, 10), (20, 30)),
            expand_x=True,
            expand_y=True,
        )
    ],
    [sg.Text("Enter file path:"), sg.Input(key="file_path")],
    [sg.Button("Firmware Update", pad=((190, 0), 25))],
]

window = sg.Window("OTA", layout, size=(500, 500))


while True:
    event, values = window.read()
    if event == sg.WINDOW_CLOSED:
        break
    if event == "Firmware Update":
        send_over_uart(values["file_path"])
window.close()
