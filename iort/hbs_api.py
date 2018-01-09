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
            logger.info('Wrote x03 to command file.')
            return cmd_file.read(1)
    else:
        logger.error('Could not write to command file')
        return "Could not open command file"


@app.route('/api/readsonar')
def read_sonar():
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x01')
            logger.info('Wrote x01 to command file.')
        return 'OK'
    else:
        logger.error('Could not write to command file')
        return "Could not write to command file"


@app.route('/api/speak', methods=['POST'])
def speak():
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x02')
            logger.info('Wrote x02 to command file.')
        return 'OK'
    else:
        logger.error('Could not write to command file')
        return "Could not write to command file"


@app.route('/api/shakehead')
def shake_head():
    logger.info('Request to shake head has been made.')
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x03')
            logger.info('Wrote x03 to command file.')
        return 'OK'
    else:
        logger.error('Could not write to command file')
        return "Could not write to command file"


if __name__ == '__main__':
    logging.basicConfig(filename='robot_api.log',
                        level=logging.DEBUG,
                        format='%(asctime)s : %(levelname)s\t %(message)s')
    logging.getLogger(__name__)
    logger.info('API starting.')
    app.run(debug=True)