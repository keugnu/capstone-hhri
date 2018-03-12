#!/usr/bin/env python3
# Author(s): Stephen Brooks

import os
import sys
import requests
import logging

from time import sleep


WELCOME = """
Welcome to the Robo-Sense control terminal, sponsored by Dewritos:
    "If you want to fuel your pro gaming workouts the right way, choose Dewritos!"
Please enter a command:
    1)  Detect distance of nearest object (in centimeters) in front of the robot
    2)  Speak
    3)  Shake head
    4)  Exit the program
"""

API_URI = 'http://127.0.0.1:5000'
VALID_COMMANDS = {
    1: 'readsonar',
    2: 'speak',
    3: 'shakehead'
}


def wait_for_data():
    logger.info("Sending request for data from the sensor.")
    req = requests.get(API_URI + '/api/getdata')
    if req.status_code == 200 and len(req.content) > 0:
        print("Sensor data recieved.")
        return req.json()
    else:
        logger.warning("No data recieved from sensor.")
        return None


def send_command(user_input, tts=None):
    if tts is not None:
        logger.info('Sending GET request with data %s ', tts)
        req = requests.get(''.join([API_URI, '/api/speak?tts=', tts]))
    else:
        logger.info('Sending GET request for \"%s\" command.', user_input)
        req = requests.get(''.join([API_URI, '/api/', VALID_COMMANDS.get(user_input)]))
    
    return req.status_code


def check_input(user_input):
    if user_input == 4:
        logger.warning('User wishes to exit. Shutting down...')
        print('Goodbye.')
        sys.exit(0)
    elif not user_input in VALID_COMMANDS:
        logger.warning('User input %s is not a valid command.', user_input)
        print('Your input is not a valid command. Try again: ')
        return None
    else:
        logger.info('User input %s was accepted.', user_input)
                           
        if user_input == 2:
            sys.stdout.write('Please provide text for the robot to speak: ')
            tts = input()
            return send_command(user_input, tts)
        else:
            status = send_command(user_input)
            if status == 200 and user_input in [1]:
                print('Waiting a maximum of 10 seconds for data from the sensor...\n')
                for i in range(10):
                    data = wait_for_data()
                    if data is not None:
                        print('Data recieved: {}'.format(data))
                        break
                    else:
                        logger.warning("Trying to get data again.")
                        sleep(1)
                if data is None:
                    print('No data recieved from the sensor.')
            return status
    logger.error('Something went wrong...')
    return None


def main():
    logger.info('Application has started.')
    
    print(WELCOME)
    while True:
        try:
            sys.stdout.write('Command: ')
            user_input = input()
            resp = check_input(int(user_input))
            if resp == None:
                pass
            elif resp != 200:
                logger.error('The request was unsuccessful. Status code: %s ', resp)
                print('The API call failed. Try again... ')
        except ValueError:
            logger.warning('User input %s is not a valid command.', user_input)
            sys.stdout.write('Your input is not a valid command. Try again: ')
            pass
    return 0


if __name__ == '__main__':
    # start_logging()
    logging.basicConfig(filename='robot_remote.log',
                        level=logging.DEBUG,
                        format='%(asctime)s : %(levelname)s\t %(message)s')
    logger = logging.getLogger(__name__)
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        print('\nKeyboardInterrupt detected. Shutting down...')
        sys.exit(0)
