from mpl_toolkits.axes_grid1 import host_subplot
import mpl_toolkits.axisartist as AA
import matplotlib.pyplot as plt
import numpy as np

sonar = host_subplot(111, axes_class=AA.Axes)
plt.subplots_adjust(right=0.75)

plt.rcParams.update({'axes.titlesize': 20})

sonar.set(title="SRF02 Sonar Sensor")

par1 = sonar.twinx()
par2 = sonar.twinx()

offset = 40
new_fixed_axis = par2.get_grid_helper().new_fixed_axis
par2.axis["right"] = new_fixed_axis(loc="right", axes=par2, offset=(offset, 0))

par2.axis["right"].toggle(all=True)

sonar.set_xlabel("Distance (cm)")
sonar.set_ylabel("Measured (cm)")
par2.set_ylabel("% Error")

sonar_cm = np.arange(30, 260, 10)
sonar_mea = np.array([30.6,38,48,58.2,68,78.8,89,98.2,108,118,129,139,149,158.8,169,178,187.2,197,206,175,226,237,247.8])
sonar_err = np.array([2,5,4,3,2.85714285714286,1.5,1.11111111111111,1.8,1.81818181818182,1.66666666666667,0.769230769230769,0.714285714285714,0.666666666666667,0.749999999999993,0.588235294117647,1.11111111111111,1.47368421052632,1.5,1.90476190476191,20.4545454545455,1.73913043478261,1.25,0.879999999999995])

p1, = sonar.plot(sonar_cm, sonar_mea, label="Measured")
p2, = par2.plot(sonar_cm, sonar_err, label="% Error")
plt.xticks(np.arange(min(sonar_cm), max(sonar_cm) + 1, 20))
plt.yticks(np.arange(30, 260, 20))

par2.set_ylim(0, 100)

sonar.legend(loc=2)

sonar.axis["left"].label.set_color(p1.get_color())
sonar.axis["left"].label.set_size(15)
sonar.axis["bottom"].label.set_size(15)
par2.axis["right"].label.set_color(p2.get_color())
par2.axis["right"].label.set_size(15)

plt.draw()
plt.show()
