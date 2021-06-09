import matplotlib as mpl
mpl.use('Agg')

import matplotlib.pyplot as plt
import numpy as np

# Plotting f:--> y(x)
# Setting the range and fineness of the x-coordinates
x = np.arange(0.001, 5, 0.01)

# Setting y(x)
y1 = x      
y2 = -x + 3 
y3 = 3 - (1 / x) 
y4 = 1 / x 
    
# Axis labels, figure size and other visual things
figsize = (4, 4)

fig = plt.figure(figsize=figsize)
ax = fig.add_subplot(1, 1, 1)

plt.xlabel('Size of object')
plt.ylabel('Surface area to Volume ratio')

ax.plot(x, y1)
ax.plot(x, y2)
ax.plot(x, y3)
ax.plot(x, y4)

ax.set_xlim([0.05, 5])
ax.set_ylim([0.05, 5])

# Exporting so that this works remotely too
plt.savefig("test.pdf")