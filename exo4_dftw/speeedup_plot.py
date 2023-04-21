import matplotlib.pyplot as plt
import numpy as np

t1 = 16.616759
threads = [1, 32, 64, 128]
speeds = [t1, t1/0.529916, t1/0.285240,  t1/0.184946] 
plt.scatter(threads, speeds)

z = np.polyfit(threads, speeds, 2)
p = np.poly1d(z)

#add trendline to plot
plt.plot(threads, p(threads), linestyle="--", color="purple")
plt.xlabel('Number of threads')
plt.ylabel('SpeedUp')
plt.show()
