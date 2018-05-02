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

# > Tweaks by Shane Synan
# Better logging

# Thanks, Internet fellow!
# http://victorlin.me/posts/2012/08/26/good-logging-practice-in-python
# Please follow this - good logging makes troubleshooting much easier :)
import json
import logging.config
# For logging configuration

#ALSA: import alsaaudio
import pyaudio
import wave
import simplejson as json
import subprocess
import os
import atexit
#import statusicon
import sys

import ProcessText
import audioop
import math
import time
import logging 

from datetime import datetime

APP_NAME='vox-launcher'
CHUNK = 1024
FORMAT = pyaudio.paInt16 # PyAudio
CHANNELS = 1
RATE = 16000
WAVE_OUTPUT_FILENAME = '/tmp/' + APP_NAME + '-recording.wav'
FLAC_OUTPUT_FILENAME = '/tmp/' + APP_NAME + '-recording.flac'
MAXRESULT=6
lo  = 2000
hi = 32000
#si = statusicon.StatusIcon()
pt = ProcessText.ProcessText()
log_lo = math.log(lo)
log_hi = math.log(hi)

def clean_up():
    ''' Clean up, clean up, everybody do your share '''
    if os.path.isfile(FLAC_OUTPUT_FILENAME):
      os.remove(FLAC_OUTPUT_FILENAME)

def send_recv():
    ''' Encode, send, and receive FLAC file '''
    #audio = open(FLAC_OUTPUT_FILENAME, 'rb').read()

    # Call pocketsphinx
    sphinx_cmd = "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:usr/local/lib pocketsphinx_continuous -infile " + WAVE_OUTPUT_FILENAME + " 2>/dev/null"
    # Remove  + " 2>/dev/null"
    # to show the pocketsphinx output
    logging.debug( "Calling pocketsphinx via " + str(sphinx_cmd))
    # Get results
    result = subprocess.check_output(sphinx_cmd, shell=True)
    # Strip newlines and whitespace on the outside
    result = result.strip()
    if result == "":
        logging.debug( "No result" )
    else:
        logging.debug( "Result: " + str(result))

    return result
    
def capture_audio(inp):
    sound = []
    
    chunk = 0
    volume = 0
    
    # Start recording at above...
    volume_threshold = 4 #1
    # Stop recording when falls below...
    volume_silence_threshold = 2 #1
    
    # 7.5, 6.9
    
    # Ubuntu 13.10 - changes in volume?
    # Original: volume_threshold = 9.9 #1 
    #           volume_silence_threshold = 9.6 #1
    silence_threshold = 8
    silence_counter = 0
    
    ## Keep track of how long capture has been going
    #capture_start = datetime.now()
    #capture_max = 1 * 60 * 60
    ## > 1 hour (hours * 60 minutes * 60 seconds)
    
    #si.change_status(True)

    #while(not si.is_paused()):
    while(True):
        try:
            #ALSA: _, data = inp.read()
            data = inp.read(CHUNK)
        #ALSA: except alsaaudio.ALSAAudioError, e:
        except IOError, e:
            data = ""
            logging.error( "Error capturing audio: " + str(e))
            exit()
        #ALSA: #_, data = inp.read()
        
        # transform data to logarithmic scale
        peak = audioop.max(data, 2)
        vu = (math.log(float(max(peak,1)))-log_lo)/(log_hi-log_lo)
        
        # transform the scale in the range 0...10
        current_volume = min(max((vu*10),0),10)
        
        # Comment out these to hide volume
        #if current_volume > 0:
        #    logging.debug( "-> volume: " + str(current_volume) )
        
        if chunk == 0:
           if current_volume < volume_threshold:
               #if current_volume != 0:
               #   logging.debug( "Dropped package with volume " + str(current_volume) )
               #if abs((capture_start - datetime.now()).total_seconds()) > capture_max:
               #    # Works around ALSA input breaking after some length of time
               #    logging.debug("1 hour elapsed, re-opening microphone")
               #    return "restart"
               # Else - ignore sound chunk
               continue
         
        else:
            if chunk>0 and volume <= volume_silence_threshold:
                silence_counter = silence_counter + 1
            else:
                silence_counter = 0
        
        if (silence_counter >= silence_threshold):
            logging.debug( "Detected " + str(silence_threshold) + " silent loop" )
            for i in range(1, silence_threshold):
                sound.pop()
            break 
            
        volume = current_volume

        chunk = chunk + 1
        sound.append(data)
        time.sleep (RATE / CHUNK / 1000.0);

    logging.debug( "Recorded " + str(chunk) + "\n" )
    
    if (chunk > 7):
        return ''.join(sound)
    else:
        return ""

def write_wav(data):
    ''' Write string of data to WAV file of specified name '''
    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(CHANNELS)
    wf.setsampwidth(2)
    wf.setframerate(RATE)
    wf.writeframes(data)
    wf.close()

''' Set up mic, capture audio, and return string of the result '''
def setup_mic():
    pAudio = pyaudio.PyAudio()
    inp = pAudio.open(format=FORMAT,
                      channels=CHANNELS,
                      rate=RATE,
                      input=True,
                      frames_per_buffer=CHUNK)
    return inp

def main():
    # Setup microphone
    inp = setup_mic()
    
    while (1):        
        command_success = False
            
        ''' Run through process of getting, converting, sending/receiving, and 
            processing data '''
  
        tstart = datetime.now()

        # code to speed test

        tend = datetime.now()
        logging.debug( "Init data capture " + str(tend - tstart) )
    
        # Capture an audio chunk
        data = capture_audio(inp)
        
        tend = datetime.now()
        logging.debug( "Finish data capture " + str(tend - tstart) )
        
        if data!="":
            # write to WAV file
            write_wav(data)
   
            tend = datetime.now()
            logging.debug( "Finish print wav " + str(tend - tstart) )
      
            #tend = datetime.now()
            #logging.debug( "Finish flac reencoding " + str(tend - tstart) )
            
            # Send and receive translation
            resp = send_recv()
            
            if resp !="":
                # Handle command here
                logging.debug( "Passing off command to parser..." )
                if pt.process_text(resp):
                    logging.info( "Success" )
                else:
                    logging.info( "No matching command" )

# Logging
#FILE-BASED: logging_directory = "logs"
#FILE-BASED: if not os.path.exists(logging_directory):
#FILE-BASED: 	os.makedirs(logging_directory)

def setup_logging(
	default_path='config-logging.json', 
	default_level=logging.DEBUG,
	env_key='LOG_CFG'
):
	"""Setup logging configuration

	"""
	path = default_path
	value = os.getenv(env_key, None)
	if value:
		path = value
	if os.path.exists(path):
		with open(path, 'r') as f:
			config = json.load(f)
		logging.config.dictConfig(config)
	else:
		logging.basicConfig(level=default_level)

#logger = logging.getLogger (__name__)
## Called at the definition of a class, or top of a function outside a class

if __name__ == '__main__':
    setup_logging ()
	# This is only called once in the main function
    atexit.register(clean_up)
    main()
