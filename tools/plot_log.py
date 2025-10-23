#!/usr/bin/env python3
"""
Plot desired vs measured from data_log.txt
Columns in data_log.txt: t, y0, y1, y2, y3, yd0, yd1, yd2, yd3, dq0, dq1, dq2, dq3, cur1, cur2, cur3, cur4
"""
import os
import sys
import math
import numpy as np
import matplotlib.pyplot as plt

LOG = 'data_log.txt'
OUTDIR = 'plots'

if not os.path.exists(LOG):
    print(f'Error: {LOG} not found in current directory')
    sys.exit(1)

data = np.loadtxt(LOG, delimiter=',')
# ensure shape
if data.ndim == 1:
    data = data.reshape(1, -1)

t = data[:,0]
meas_y = data[:,1:5]

# Read desired from log (columns 5-8)
if data.shape[1] >= 9:
    yd = data[:,5:9]
else:
    # Fallback to old analytic reconstruction if log format is old
    print("Warning: Old log format detected, reconstructing desired trajectory analytically")
    omega = 0.5
    yd = np.zeros_like(meas_y)
    yd[:,0] = 0.1 + 0.04*np.cos(omega*t)
    yd[:,1] = 0.05*np.sin(omega*t)
    yd[:,2] = 0.1
    yd[:,3] = np.pi/2

os.makedirs(OUTDIR, exist_ok=True)

labels = ['x','y','z','theta']
for i in range(4):
    plt.figure(figsize=(8,4))
    plt.plot(t, meas_y[:,i], label='measured', linewidth=1)
    plt.plot(t, yd[:,i], label='desired', linewidth=1, linestyle='--')
    plt.xlabel('time (s)')
    plt.ylabel(labels[i])
    plt.title(f'Measured vs Desired - {labels[i]}')
    plt.legend()
    plt.grid(True)
    out = os.path.join(OUTDIR, f'{labels[i]}.png')
    plt.tight_layout()
    plt.savefig(out)
    print('Saved', out)
    plt.close()

print('All plots saved to', OUTDIR)
