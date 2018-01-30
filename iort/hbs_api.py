#!/usr/bin/env python3
# Author(s): Stephen Brooks

import os
import sys
import logging
from flask import Flask, request

COMMAND_FILE_PATH = r'/home/stephen/Projects/capstone-hhri/hbs/api/command.bin'
SPEECH_KW_PATH = r'/home/stephen/Projects/capstone-hhri/hbs/api/tts.txt'
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
        return b'\xFF'


@app.route('/api/gettts')
def get_tts():
    if os.path.exists(SPEECH_KW_PATH):
        with open(SPEECH_KW_PATH, 'r') as spch_file:
            logger.info('Reading from tts file.')
            return spch_file.readline()
    else:
        logger.error('Could not read from tts file.')
        return b'\xFF'


@app.route('/api/readsonar')
def read_sonar():
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x01')
            logger.info('Wrote x01 to command file.')
        return b'\x00'
    else:
        logger.error('Could not write to command file')
        return b'\xFF'


@app.route('/api/speak')
def speak():
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x02')
            logger.info('Wrote x02 to command file.')
        with open(SPEECH_KW_PATH, 'w') as spch_file:
            spch_file.write(request.args.get('tts'))
        return b'\x00'
    else:
        logger.error('Could not write to command file')
        return b'\xFF'


@app.route('/api/shakehead')
def shake_head():
    logger.info('Request to shake head has been made.')
    if os.path.exists(COMMAND_FILE_PATH):
        with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
            cmd_file.write(b'\x03')
            logger.info('Wrote x03 to command file.')
        return b'\x00'
    else: 
        logger.error('Could not write to command file')
        return b'\xFF'


if __name__ == '__main__':
    logging.basicConfig(filename='robot_api.log',
                        level=logging.DEBUG,
                        format='%(asctime)s : %(levelname)s\t %(message)s')
    logger = logging.getLogger(__name__)
    logger.info('API starting.')
    app.run(debug=True)