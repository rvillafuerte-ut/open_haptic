#!/usr/bin/env python3
"""
Plot desired vs measured from data_log.txt
Columns in data_log.txt: t, y0-3(measured), yd0-3(desired), dy0-3(measured vel), dyd0-3(desired vel), u0-3(control), dq0-3, cur1-4
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

# Read desired position from log (columns 5-8)
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

# Read velocities and control if available (new format)
# Format: t(1), y(4), yd(4), dy(4), dyd(4), u(4), dq(4), cur(4) = 29 columns
has_velocities = data.shape[1] >= 17
has_control = data.shape[1] >= 21

if has_velocities:
    dy = data[:,9:13]
    dyd = data[:,13:17]
else:
    dy = None
    dyd = None
    
if has_control:
    u = data[:,17:21]
else:
    u = None

os.makedirs(OUTDIR, exist_ok=True)

labels = ['x','y','z','theta']
vel_labels = ['dx','dy','dz','dtheta']
control_labels = ['u1','u2','u3','u4']

# Plot positions (measured vs desired)
for i in range(4):
    plt.figure(figsize=(8,4))
    plt.plot(t, meas_y[:,i], label='measured', linewidth=1)
    plt.plot(t, yd[:,i], label='desired', linewidth=1, linestyle='--')
    plt.xlabel('time (s)')
    plt.ylabel(labels[i])
    plt.title(f'Position: Measured vs Desired - {labels[i]}')
    plt.legend()
    plt.grid(True)
    out = os.path.join(OUTDIR, f'{labels[i]}.png')
    plt.tight_layout()
    plt.savefig(out)
    print('Saved', out)
    plt.close()

# Plot velocities (measured vs desired) if available
if has_velocities and dy is not None and dyd is not None:
    for i in range(4):
        plt.figure(figsize=(8,4))
        plt.plot(t, dy[:,i], label='measured velocity', linewidth=1)
        plt.plot(t, dyd[:,i], label='desired velocity', linewidth=1, linestyle='--')
        plt.xlabel('time (s)')
        plt.ylabel(vel_labels[i])
        plt.title(f'Velocity: Measured vs Desired - {vel_labels[i]}')
        plt.legend()
        plt.grid(True)
        out = os.path.join(OUTDIR, f'{vel_labels[i]}.png')
        plt.tight_layout()
        plt.savefig(out)
        print('Saved', out)
        plt.close()
else:
    print('Velocities not available in log file')

# Plot control law if available
if has_control and u is not None:
    for i in range(4):
        plt.figure(figsize=(8,4))
        plt.plot(t, u[:,i], label=f'control law {control_labels[i]}', linewidth=1, color='red')
        plt.xlabel('time (s)')
        plt.ylabel(control_labels[i] + ' (Nm)')
        plt.title(f'Control Law - {control_labels[i]}')
        plt.legend()
        plt.grid(True)
        out = os.path.join(OUTDIR, f'{control_labels[i]}.png')
        plt.tight_layout()
        plt.savefig(out)
        print('Saved', out)
        plt.close()
else:
    print('Control law not available in log file')

print('All plots saved to', OUTDIR)
