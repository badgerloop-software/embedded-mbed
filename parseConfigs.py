#!/bin/python3

import yaml


# Get information from configuration files
with open('tests.yml', 'r') as testFile, open('setup.yml', 'r') as setupFile:
	testCfg = yaml.safe_load(testFile)
	setupCfg = yaml.safe_load(setupFile)


# Get target and ensure that it is consistent across the configuration files
target = setupCfg['setup']['target']
assert target == list(testCfg.keys())[0], "Target in setup.yml ({}) does not match the root key in tests.yml ({})".format(target, list(testCfg.keys())[0])


# Stores lines of generated definitions header file
definitions=[]
lineNum = 1


# Insert definitions for devices and drivers macros
# These macros will be used throughout the project whenever test setup-related info is needed
for setupCat in ['devices', 'drivers']:
	if isinstance(setupCfg['setup'][setupCat], dict):
		definitions.insert(lineNum, '{}// Define macros for {}\n'.format('\n' if setupCat != 'devices' else '', setupCat))
		lineNum += 1
		for setup, setupInfo in setupCfg['setup'][setupCat].items():
			for attr, val in setupInfo.items():
				# If val is a list, make it a string and reformat it as a C array (i.e. {...} instead of [...])
				if isinstance(val, list):
					val = '{' + str(val)[1:-1] + '}'
				definitions.insert(lineNum, '#define {}_{}    {}\n'.format(setup.upper(), attr.upper(), val))
				lineNum += 1

# Insert definitions for test macros
# These macros will be used in testRunner.cpp to determine what tests should be run,
# as well as in any source files that have test-dependent configurations
if isinstance(testCfg[target], dict):
	definitions.insert(lineNum, '\n// Define macros for tests\n')
	lineNum += 1
	for testCat, tests in testCfg[target].items():
		startLine=lineNum
		for test, testType in tests.items():
			if testType != 'disabled':
				definitions.insert(lineNum, '#define {}_{}    \"{}\"\n'.format(testCat.upper(), test.upper(), testType.upper()))
				lineNum += 1
		# Only insert the TEST_<TEST_CAT> line if one or more of the tests within that category are not disabled
		if startLine < lineNum:
			definitions.insert(startLine - 1, '// Start {} test definitions\n#define TEST_{}\n'.format(testCat, testCat.upper()))
			lineNum += 1

# Create/Replace the contents of definitions.h with the generated lines
with open('definitions.h', 'w') as definitionsFile:
	definitionsFile.writelines(definitions)
