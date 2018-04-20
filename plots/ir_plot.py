from mpl_toolkits.axes_grid1 import host_subplot
import mpl_toolkits.axisartist as AA
import matplotlib.pyplot as plt
import numpy as np

ir = host_subplot(111, axes_class=AA.Axes)
plt.subplots_adjust(right=0.75)

plt.rcParams.update({'axes.titlesize': 35})

ir.set(title="VL53L0X IR Laser TOF Sensor")

par1 = ir.twinx()
par2 = ir.twinx()

offset = 40
new_fixed_axis = par2.get_grid_helper().new_fixed_axis
par2.axis["right"] = new_fixed_axis(loc="right", axes=par2, offset=(offset, 0))

par2.axis["right"].toggle(all=True)

ir.set_xlabel("Distance (mm)")
ir.set_ylabel("Measured (mm)")
par2.set_ylabel("% Error")

ir_mm = np.arange(20, 240, 10)
ir_mea = np.array([11.8,18.2,57.6,86,91.2,106.6,92.6,127,160.4,147,157.4,181.6,180.8,191.4,184.8,213,229,225.4,239,246.4,260.2,262])
ir_err = np.array([41,39.3333333333333,44,72,52,52.2857142857143,15.75,41.1111111111111,60.4,33.6363636363636,31.1666666666667,39.6923076923077,29.1428571428572,27.6,15.5,25.2941176470588,27.2222222222222,18.6315789473684,19.5,17.3333333333333,18.2727272727273,13.9130434782609])

p1, = ir.plot(ir_mm, ir_mea, label="Measured")
p2, = par2.plot(ir_mm, ir_err, label="% Error")
plt.xticks(np.arange(min(ir_mm), max(ir_mm) + 1, 10))
plt.yticks(np.arange(10, 270, 10))

par2.set_ylim(0, 100)

ir.legend(loc=2)

ir.axis["left"].label.set_color(p1.get_color())
ir.axis["left"].label.set_size(30)
ir.axis["bottom"].label.set_size(30)
par2.axis["right"].label.set_color(p2.get_color())
par2.axis["right"].label.set_size(30)

plt.draw()
plt.show()
