import random
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

plt.style.use('fivethirtyeight')

x_vals = [0]
y1_vals = [0]
y2_vals = [0]



sample = 0

def animate(i):
    global sample 
    sample +=1
    x_vals.append(sample)
    y1_vals.append(random.randint(0, 5))
    y2_vals.append(random.randint(0, 5))
    

    plt.cla()
    plt.plot(x_vals[0:len(y1_vals)], y1_vals, label='Channel 1')
    plt.plot(x_vals, y2_vals, label='Channel 2')

    plt.legend(loc='upper left')
    plt.tight_layout()


ani = FuncAnimation(plt.gcf(), animate, interval=100)

plt.tight_layout()
plt.show()