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
"""



def main():
    print(WELCOME)
    return 0

if __name__ == '__main__':
    sys.exit(main())