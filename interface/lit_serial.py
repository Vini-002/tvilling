"""
Streamlit Serial plot
"""
import os
import subprocess

import numpy as np
import serial
import streamlit as st
from serial import SerialException

HEADER_TEXT = """
# Streamlit Serial plot
### Plots values from a serial port
Expecting comma separated format: `0.00, 0.00, ...`
"""

st.write(HEADER_TEXT)

LIST_PORTS = "python -m serial.tools.list_ports -q"

# get used port:
with subprocess.Popen(LIST_PORTS, shell=True, stdout=subprocess.PIPE) as proc:
    # this could be a list of active ports
    OUTPUT = proc.communicate()[0].decode("utf-8").split(os.linesep)

active_ports = [x.strip() for x in OUTPUT if x]

# display port selection
selected_port = st.sidebar.selectbox(
    'Select Port',
    active_ports
)

selected_baudrate = st.sidebar.selectbox(
    'Select Baudrade',
    (4800, 9600, 19200, 38400, 57600, 115200),
    index=1
)

if not active_ports:
    st.warning("No serial devices connected")
else:
    # initialize serial
    ser = serial.Serial()
    ser.baudrate = selected_baudrate
    ser.port = selected_port
    try:
        ser.open()
    except SerialException as e:
        st.error(f"could not open port {selected_port}. Unplug and try again")
        st.error(e)


# read first line to detect input
# ... in the future
# for now comma separated values will be assumed

channel_selection = st.multiselect(
    "Choose channels to display", [], None
)

chart = st.line_chart()

VALUE_SEPARATOR = ","
curr_val_text = st.text("current value: ")

while True:
    # this section contains input formatting, so things might go wrong
    try:
        line = ser.readline().decode("utf-8")
        channels = line.split(VALUE_SEPARATOR)

        # number of channels
        no_channels = len(channels)

        vals = np.asarray(channels, dtype=float)
        curr_val_text.text(f"Current values {vals}")
        shape = (-1, 1) if no_channels == 1 else (1, no_channels)
        data = vals.reshape(shape)
        chart.add_rows(data)

    except Exception as e:
        # drop bad reads
        st.text(f"something went wrong when processing line:\n\t{line}")
