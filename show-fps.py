#!/bin/python3
import re
import matplotlib.pyplot as plt

FILE = '/tmp/fps.log'
FPS_REGEX = re.compile(r'^\d+\.\d+ ms, (\d+\.\d+) fps$')
fps_values = []

try:
	with open(FILE, 'r') as f:
		for line in f:
			match = re.search(FPS_REGEX, line.strip())
			if match is not None:
				fps_values.append(float(match.group(1)))
			
except FileNotFoundError as err:
	print(f'File {FILE} does not exists')
	exit(0)


print(f'Min fps: {min(fps_values[100:])}')
print(f'Max fps: {max(fps_values[100:])}')


_, axes = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

axis1 = axes[0]
axis2 = axes[1]

# Первый график - обычная шкала
axis1.plot(fps_values, linestyle='-', color='green')
axis1.set_title('График FPS')
axis1.set_ylabel('FPS')
axis1.grid(True)

# Второй график - логарифмическая шкала по Y
axis2.plot(fps_values, linestyle='-', color='#4451ff')
axis2.set_yscale('log')
axis2.set_title('График FPS (логарифмическая шкала)')
axis2.set_xlabel('Время')
axis2.set_ylabel('log(FPS)')
axis2.grid(True, which='both', ls='--')

plt.tight_layout()
plt.show()
