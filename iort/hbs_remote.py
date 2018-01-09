#!/usr/bin/env python
# Author(s): Stephen Brooks

import os
import sys
import requests
import logging


WELCOME = """
Welcome to the Robo-Sense control terminal, sponsored by Dewritos:
    "If you want to fuel your pro gaming workouts the right way, choose Dewritos!"

Please enter a command:

    1)  Detect distance of nearest object in front of the robot
    2)  Speak
    3)  Shake head
    4)  Exit the program
"""

API_URI = 'http://localhost.com:5000'
VALID_COMMANDS = {
    1: 'readsonar',
    2: 'speak',
    3: 'shakehead'
}


def send_command(user_input, tts=None):
    logger = logging.getLogger(__name__)
    
    if tts is not None:
        logger.info('Sending POST request with data %s ', tts)
        req = requests.post(''.join([API_URI, '/api/speak']), data={'speech':tts})
    else:
        logger.info('Sending GET request for \"%s\" command.', user_input)
        req = requests.get(''.join([API_URI, '/api/', VALID_COMMANDS.get(user_input)]))
    
    return req.status_code


def check_input(user_input):
    logger = logging.getLogger(__name__)
    
    if user_input == 4:
        logger.warning('User wishes to exit. Shutting down...')
        print('Goodbye.')
        sys.exit(0)
    elif not user_input in VALID_COMMANDS:
        logger.warning('User input %s is not a valid command.', user_input)
        sys.stdout.write('Your input is not a valid command. Try again: ')
    else:
        logger.info('User input %s was accepted.', user_input)
        if user_input == 2:
            sys.stdout.write('Please provide text for the robot to speak: ')
            tts = input()
            return send_command(user_input, tts)
        else:
            return send_command(user_input)
    logger.error('Something went wrong...')
    return None


def main():
    # start_logging()
    logger = logging.getLogger(__name__)
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
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        print('\nKeyboardInterrupt detected. Shutting down...')
        sys.exit(0)
