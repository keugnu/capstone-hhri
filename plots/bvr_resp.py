import matplotlib.pyplot as plt; plt.rcdefaults()
import numpy as np
import matplotlib.pyplot as plt
 
objects = ('Pat on back', 'Sonar via IoT', 'Temperature via IoT', 'TTS via IoT', 'Shake Head via IoT', 'Hugging')
y_pos = np.arange(len(objects))
timings = [0.001, 1.4, 0.5, 2,4, 0.3, 0.001]
 
plt.bar(y_pos, timings, align='center', alpha=0.5)
plt.xticks(y_pos, objects)
plt.ylabel('Elapsed Time (s)', fontsize=30)
plt.title('Response Time of Interactions', fontsize=35)

plt.tick_params(axis='x', which='major', labelsize=25)
 
plt.show()
