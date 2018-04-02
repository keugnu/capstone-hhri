#!/usr/bin/python
#
# ProcessText.py
# Copyright (C) Pilolli 2012 <pilolli.pietro@gmail.com>
# 
# vox-launcher is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# vox-launcher is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

COMMAND_FILE = '/home/ubuntu/Desktop/LHV/voice-commands.conf'

import os
import time
import subprocess
import logging

class ProcessText():

    def __init__(self):
        self.is_grid_running = False

    def program_exists(self, fname):
        for p in os.environ['PATH'].split(os.pathsep):
            if os.path.isfile(os.path.join(p, fname)):
                return True
        return False

    def get_matching_command(self, t):
        fileName = COMMAND_FILE
        if not os.path.isfile(fileName):
            logging.error("Failed to open " + fileName)
            return ""
        
        file = open(fileName,"r")
        ln = 0
    
        for line in file:
            ln = ln + 1
            if line.startswith('#') or line.isspace():
                continue
            else:
                temp=line.split("->")
                if len(temp) == 2:
                    exp = temp[0].strip()
                    if t == exp:
                        command = temp[1].strip()
                        file.close()
                        return command
                else:
                    logging.debug( "can't parse line " + str(ln) )

        file.close()
        return ""

    def open_program(self, t):
        command = t

        new_command = self.get_matching_command(t)
        if new_command == "":
            # Try again, lower-case
            new_command = self.get_matching_command(t.lower())

        if new_command!="":
            command = new_command
        # Edited by Shane: don't allow running arbitrary programs with voice search o.O
        else:
            return False
        
        logging.debug( "Command " + command )
        progname = command.split(" ")[0]
        if self.program_exists(progname):
            subprocess.Popen(command, shell=True)
            return True
        
        return False
            
    def process_text(self, text):
        ''' Take in a tuple of the confidence and result and do the appropriate 
            action or also the text'''

        # Process the input text
        logging.debug("Handling text '" + text + "'")
        # Append command to text file for later usage, too
        with open("output.txt", "a") as outfile:
            outfile.write(text)
        logging.debug("Checking for command...")
        status = self.open_program(text)
        return status
