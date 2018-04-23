import matplotlib
import matplotlib.pyplot as plt
import numpy as np

# Data for plotting

fsr_mea = np.array([3.301,3.301,3.247,3.2,3.17,3.14,3.06,3.04,2.98,2.94,2.92,2.87,2.84,2.8,2.76,2.74,2.7,2.66,2.63,2.61,2.58,2.54,2.51,2.49,2.4631])
fsr_x = np.array([0,0.0556227,0.1112454,0.1668681,0.2224908,0.2781135,0.3337362,0.3893589,0.4449816,0.5006043,0.556227,0.6118497,0.6674724,0.7230951,0.7787178,0.8343405,0.8899632,0.9455859,1.0012086,1.0568313,1.112454,1.1680767,1.2236994,1.2793221,1.3349448,])

# Note that using plt.subplots below is equivalent to using
# fig = plt.figure() and then ax = fig.add_subplot(111)
fig, ax = plt.subplots()
ax.plot(fsr_x, fsr_mea, linewidth=6)

ax.set(xlabel='Applied Force (N)', ylabel='Measured Voltage (V)',
       title='FSR Sensitivity')

ax.xaxis.label.set_size(40)
ax.yaxis.label.set_size(40)
ax.title.set_size(45)

plt.xticks(fontsize=25)
plt.yticks(fontsize=25)


ax.grid()

plt.show()
