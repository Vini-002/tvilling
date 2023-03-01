"""
Python interface for control of a single axis robot and demonstration of PID
"""
import subprocess
import os

import serial
import streamlit as st

from serial import SerialException
import numpy as np
import pandas as pd


# arduino = serial.Serial('/dev/ttyUSB0')

"""
# Tvilling
---
## PID Tuning
"""

cols = st.columns(3)
cols[0].number_input("P", value=1., step=0.05)
cols[1].number_input("I", value=0., step=0.05)
cols[2].number_input("D", value=0., step=0.05)

if st.button("RUN"):
    pass

"""
---
## Position tracking
"""

st.slider("Target position", 0, 180, 90)
st.button("Go to position")

st.line_chart([3, 24, 53, 72, 79, 85, 87, 89, 90, 90])
























"""
    # Streamlit Serial plot
    plots values that are readable from a given port\n
    Expecting comma separated format: _"0.00,0.00, ..."_\n
    Can also be json like: _"key:value, next_key: next_value, ..."_

"""

# get used port:
proc = subprocess.Popen(
    "python -m serial.tools.list_ports -q",
    shell=True,
    stdout=subprocess.PIPE
    )
# this could be a list of active ports
output = proc.communicate()[0]
output = output.decode("utf-8")
active_ports = [x.strip() for x in output.split(os.linesep)]

# display port selection
selected_port = st.sidebar.selectbox(
    'Select Port',
    active_ports,
    index=0)

selected_baudrate = st.sidebar.selectbox(
    'Select Baudrade',
    (4800, 9600, 19200, 38400, 57600, 115200),
    index=1
)

# initialize serial
ser = serial.Serial()
ser.baudrate = selected_baudrate
ser.port = selected_port
try:
    ser.open()
except SerialException as e:
    st.error(f"could not open port {selected_port}. Unplug and try again")
    st.button("try again")
    st.error(e)
    pass


# read first line to detect input
# ... in the future
# for now comma separated values will be assumed

channel_selection = st.multiselect(
    "Choose channels to display", [], None
)

chart = st.line_chart()


value_separator = ","
curr_val_text = st.text("current value: ")

curr_frame = st.dataframe()

curr_err_text = st.warning("no Worries")
curr_err_detail = st.text("really. All is great!")

"""
    ## Todo:
    - a slider for selection of number of rows to be displayed
    - some form of input format selection
    - an export functionality
"""

idx = 0
while True:
    # this section contains input formatting, so things might go wrong
    try:
        line = ser.readline().decode("utf-8")
        channels = line.split(value_separator)

        # number of channels
        no_channels = len(channels)
        # check if input format is json like
        if len(channels[0].split(":")) == 2:
            # assuming key, value pairs formatted like
            # key : value
            channels = [channel.split(":") for channel in line.split(",")]

            # wrap as np array for easier access
            magic = np.asarray(channels).transpose()

            # convert access first column
            values = np.asarray(magic[1], dtype=float)

            # strip whitespace
            col_names = np.char.strip(magic[0])

            # check if time is passed as parameter
            time_channel_names = np.asarray([
                "_millies",
                "millis",
                "_millis",
                "time"])
            idx_arr = np.in1d(col_names, time_channel_names)
            if any(idx_arr):
                if sum(idx_arr) > 2:
                    raise ValueError(
                        f"multiple time channels:{col_names[idx_arr]}")
                else:
                    # retrieve index value
                    idx = values[idx_arr]
                    # keep column names and values where is not index
                    values = values[~idx_arr]
                    col_names = col_names[~idx_arr]
                    data = pd.DataFrame(
                        [values.tolist()],
                        columns=col_names,
                        index=idx)
            else:
                data = pd.DataFrame([values], columns=col_names, index=[idx])
                idx += 1

            chart.add_rows(data)

        else:
            vals = np.asarray(channels, dtype=float)
            curr_val_text.text("Current values {}".format(vals))
            shape = (-1, 1) if no_channels == 1 else (1, no_channels)
            data = vals.reshape(shape)
            chart.add_rows(data)
            pass

    except Exception as e:
        # drop bad reads
        curr_err_text.text(f"something went wrong when processing line:\n\t{line}")
        curr_err_detail.text(str(e))
        pass

    pass

