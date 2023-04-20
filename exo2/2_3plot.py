import numpy as np
import matplotlib.pyplot as plt

# Data for plotting
labels = ["static", "dynamic", "guided"]
average_bw = [20108.579176, 19724.558093, 20488.597358]
standard_deviation_bw = [127.270531, 349.466628, 587.777823]


if __name__ == '__main__':
    fig, ax = plt.subplots()
    ax.plot(labels, average_bw, 'o-')
    ax.errorbar(labels, average_bw, yerr=standard_deviation_bw, fmt='o-')
    ax.set(xlabel='Schedule', ylabel='Average Bandwidth (MB/s)',
           title='Average Bandwidth vs Schedule')
    ax.grid()
    fig.savefig("2_3plot.png")
    plt.show()