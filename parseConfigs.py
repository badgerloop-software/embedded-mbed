#!/bin/python3

import yaml


# Get necessary information from configuration files and CMakeLists.txt
# Additionally, clear existing macros between start and end markers in CMakeLists.txt
with open('tests.yml', 'r') as testFile, open('setup.yml', 'r') as setupFile, open('CMakeLists.txt', 'r') as cmakeFile:
	testCfg = yaml.safe_load(testFile)
	setupCfg = yaml.safe_load(setupFile)
	initCmake = cmakeFile.readlines()
	
	# Iterate through CMakeLists.txt lines and
	#   - Add all lines to cmake, except for those between the start and end macros markers
	#   - Store the line number of the start macros marker
	cmake = []
	keep = True
	for lnum, line in enumerate(initCmake):
		if keep:
			cmake.append(line)
			if line == '# --- START MACROS ---\n':
				lineNum = lnum + 1
				keep = False
		elif line == '# ---- END MACROS ----\n':
			cmake.append(line)
			keep = True


# Get target and ensure that it is consistent across the configuration files
target = setupCfg['setup']['target']
assert target == list(testCfg.keys())[0], "Target in setup.yml ({}) does not match the root key in tests.yml ({})".format(target, list(testCfg.keys())[0])


# Insert definitions for devices and drivers macros
# These macros will be used throughout the project whenever test setup-related info is needed
for setupCat in ['devices', 'drivers']:
	if isinstance(setupCfg['setup'][setupCat], dict):
		cmake.insert(lineNum, '\n# Define macros for {}\n'.format(setupCat))
		lineNum += 1
		for setup, setupInfo in setupCfg['setup'][setupCat].items():
			for attr, val in setupInfo.items():
				# If val is a list, make it a string and reformat it as a C array (i.e. {...} instead of [...])
				if isinstance(val, list):
					val = '{' + str(val)[1:-1] + '}'
				cmake.insert(lineNum, 'ADD_DEFINITIONS(-D{}_{}={})\n'.format(setup.upper(), attr.upper(), val))
				lineNum += 1

# Insert definitions for test macros
# These macros will be used in testRunner.cpp to determine what tests should be run
if isinstance(testCfg[target], dict):
	cmake.insert(lineNum, '\n# Define macros for tests\n')
	lineNum += 1
	for testCat, tests in testCfg[target].items():
		for test, testType in tests.items():
			if testType != 'disabled':
				cmake.insert(lineNum, 'ADD_DEFINITIONS(-D{}_{})\n'.format(testCat.upper(), test.upper()))
				lineNum += 1

# Add empty line before end macros marker for styling purposes
cmake.insert(lineNum, '\n')

# Replace the contents of CMakeLists.txt with the modified lines
with open('CMakeLists.txt', 'w') as cmakeFile:
	cmakeFile.writelines(cmake)


# Output the target so that it can be used to check target availability in the workflow
print(target)
