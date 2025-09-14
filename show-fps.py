#!/bin/python3
import re
import matplotlib.pyplot as plt

FILE = '/tmp/fps.log'
NUM = r'\d+\.\d+'
FPS_REGEX = re.compile(rf'^({NUM}).*?({NUM}).*?({NUM}).*$')
fps = []
fps_render = []
fps_postprocess = []

try:
	with open(FILE, 'r') as f:
		for line in f:
		
			match = re.search(FPS_REGEX, line.strip())
			if match is not None:
				fps.append(float(match.group(1)))
				fps_render.append(float(match.group(2)))
				fps_postprocess.append(float(match.group(3)))
			
except FileNotFoundError as err:
	print(f'File {FILE} does not exists')
	exit(0)

def print_limits(values, name):
	print(f'Min {name}: {min(values)}')
	print(f'Max {name}: {max(values)}')
	print(f'Avg {name}: {round(sum(values) / len(values), 2)}')

	values_since_100 = values[100:]
	print(f'Min {name} from 100 frame: {min(values_since_100)}')
	print(f'Max {name} from 100 frame: {max(values_since_100)}')
	print(f'Avg {name} from 100 frame: {round(sum(values_since_100) / len(values_since_100), 2)}')

print_limits(fps, 'fps')
print_limits(fps_render, 'fps render')
print_limits(fps_postprocess, 'fps postprocess')


_, axes = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

axis1 = axes[0]
axis2 = axes[1]
axis3 = axes[2]

# Первый график - fps
axis1.plot(fps, linestyle='-', color='green')
axis1.set_title('График FPS')
axis2.set_xlabel('Время')
axis1.set_ylabel('FPS')
axis1.grid(True)

# Второй график - fps_render
axis2.plot(fps_render, linestyle='-', color='#4451ff')
axis2.set_title('График FPS (render)')
axis2.set_xlabel('Время')
axis2.set_ylabel('FPS')
axis2.grid(True, which='both', ls='--')

# Третий график - fps_postprocess
axis3.plot(fps_postprocess, linestyle='-', color='#4451ff')
axis3.set_title('График FPS (postprocess)')
axis3.set_xlabel('Время')
axis3.set_ylabel('FPS')
axis3.grid(True, which='both', ls='--')

plt.tight_layout()
plt.show()
