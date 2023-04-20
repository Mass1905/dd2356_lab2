import numpy as np
import matplotlib.pyplot as plt

# Data for plotting
threads = [1, 32, 64, 128]
copy_bw = [18581.0, 22026.7, 20202.0, 20274.0]
scale_bw = [19697.3, 22598.6, 21545.9, 20528.2]
add_bw = [23692.2, 24301.3, 23057.2, 23425.9]
triad_bw = [23437.3, 24415.1, 23520.0, 23144.2]
labels = ['Copy', 'Scale', 'Add', 'Triad']

standard_deviation_bw = [172.402876, 300.26985, 313.131655, 203.634305]


if __name__ == '__main__':
    fig, ax = plt.subplots()
    ax.plot(threads, copy_bw, label='Copy')
    ax.plot(threads, scale_bw, label='Scale')
    ax.plot(threads, add_bw, label='Add')
    ax.plot(threads, triad_bw, label='Triad')
    ax.set(xlabel='Number of Threads', ylabel='Bandwidth (MB/s)',
           title='Bandwidth vs. Number of Threads')
    ax.grid()
    ax.legend()
    fig.savefig("bandwidth_threads.png")

    fig, ax = plt.subplots()
    ax.errorbar(threads, copy_bw, yerr=standard_deviation_bw, fmt='o-')
    ax.set(xlabel='Operation', ylabel='Bandwidth (MB/s)',
              title='Bandwidth vs. Number of Threads (copy kernel)')
    ax.grid()
    fig.savefig("bandwidth_threads_copy.png")


    plt.show()