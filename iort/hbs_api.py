#!/usr/bin/env python3
# Author(s): Stephen Brooks

import os
import sys
import logging
from flask import Flask

COMMAND_FILE_PATH = r'/home/stephen/Projects/capstone-hhri/hbs/api/command'
app = Flask(__name__)


@app.route('/api')
def index():
    return "Hello World!"


@app.route('/api/getcommand')
def get_command():
    if os.path.exists(COMMAND_FILE_PATH):
       with open(COMMAND_FILE_PATH, 'rb') as cmd_file:
            return cmd_file.read(1)
    else:
        return "Could not open command file"


@app.route('/api/readsonar')
def read_sonar():
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x01')
        return 'OK'
    else:
        return "Could not write to command file"


@app.route('/api/speak', methods=['POST'])
def speak():
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x02')
        return 'OK'
    else:
        return "Could not write to command file"


@app.route('/api/shakehead')
def shake_head():
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x03')
        return 'OK'
    else:
        return "Could not write to command file"


if __name__ == '__main__':
    app.run(debug=True)