/* Test file for I2S
*  Compiled using: g++ LHV_I2S_Test1.cpp -o A -lasound
*  must put -lasound at end to link or else this code will not work
*  Run using: ./A
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <alsa/asoundlib.h>
#include <i2s.h>

using std::cout;
using std::endl;

// Main method
int main()
{
  int val;

 
  // Just hello
  cout << "ALSA library version: " << SND_LIB_VERSION_STR;
  cout << "\n";
  
  cout << "PCM stream types: \n";
  for (val = 0; val <= SND_PCM_STREAM_LAST; val++)
  {
    cout << snd_pcm_stream_name((snd_pcm_stream_t)val) << "\n";
  }

  cout << "\nPCM access types: \n";
  for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
  {
    cout << snd_pcm_access_name((snd_pcm_access_t)val) << "\n";
  }

  cout << "\nPCM formats: \n";
  for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
  {
    if (snd_pcm_format_name((snd_pcm_format_t)val) != NULL)
    {
      cout << snd_pcm_format_name((snd_pcm_format_t)val) << " (" << snd_pcm_format_description((snd_pcm_format_t)val) << ")\n";
    }
  }

  cout << "\nPCM subformats: \n";
  for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++)
  {
    cout << snd_pcm_subformat_name((snd_pcm_subformat_t)val) << " (" << snd_pcm_subformat_description((snd_pcm_subformat_t)val) << ")\n";
  }

  cout << ("\nPCM states:\n");
  for (val = 0; val <= SND_PCM_STATE_LAST; val++)
  {
    cout << snd_pcm_state_name((snd_pcm_state_t)val) << "\n";
  }

  return 0;
}
