#!/usr/bin/env python3
# Author(s): Stephen Brooks

import os
import sys
import logging
from flask import Flask, request

COMMAND_FILE_PATH = r'/home/ubuntu/.git/capstone-hhri/hbs/api/command.bin'
SPEECH_KW_PATH = r'/home/ubuntu/.git/capstone-hhri/hbs/api/tts.txt'
app = Flask(__name__)

""" Error return 0
    Success return 255
    Requests:
        1   :   readsonar
        2   :   speak
        3   :   shakehead
"""

@app.route('/api')
def index():
    return "Hello World!"


@app.route('/api/getcommand')
def get_command():
    if os.path.exists(COMMAND_FILE_PATH):
       with open(COMMAND_FILE_PATH, 'rb') as cmd_file:
            logger.info('Reading from command file.')
            return "Hello World"
    else:
        logger.error('Command file does not exist.')
        return b'\x00'


@app.route('/api/gettts')
def get_tts():
    if os.path.exists(SPEECH_KW_PATH):
        with open(SPEECH_KW_PATH, 'r') as spch_file:
            logger.info('Reading from tts file.')
            return spch_file.readline()
    else:
        logger.error('TTS file does not exist.')
        return b'\x00'


@app.route('/api/readsonar')
def read_sonar():
    logger.info("Request to read sonar sensor has been made.")
    with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
        logger.info('Writing 0x01 to command file.')
        cmd_file.write(b'\x01')
    return b'\xFF'


@app.route('/api/speak')
def speak():
    logger.info('Request to use TTS has been made')
    with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
        logger.info('Writing 0x02 to command file.')
        cmd_file.write(b'\x02')
    with open(SPEECH_KW_PATH, 'w') as spch_file:
        logger.info('Writing \"%s\" to TTS file.', request.args['tts'])
        spch_file.write(request.args['tts'] + '\n')
    return b'\xFF'


@app.route('/api/shakehead')
def shake_head():
    logger.info('Request to shake head has been made.')
    with open(COMMAND_FILE_PATH, 'wb') as cmd_file:
        logger.info('Writing 0x03 to command file.')
        cmd_file.write(b'\x03')
    return b'\xFF'


if __name__ == '__main__':
    logging.basicConfig(filename='robot_api.log',
                        level=logging.DEBUG,
                        format='%(asctime)s : %(levelname)s\t %(message)s')
    logger = logging.getLogger(__name__)
    logger.info('API starting.')
    app.run(debug=True, host='0.0.0.0')
