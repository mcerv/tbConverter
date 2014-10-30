import fnmatch
import os
import re

#get the current directory name
#currentdir =  os.getcwd().split(os.sep)[-1]
#print currentdir


#rename the files so that they have a four-digit number in the beginning.
#read the first number and add zeros if necessary
nums = []
cnt = 0
prefix = 0
#nums = (re.findall(filename)

#dir = 'data/lithium_renamed'
#dir = 'data/lithium_renamed_shorter'
dir = '../data/2014-08-04'
#dir = 'data/lithium_renamed_short'
#dir = 'rawdata/dataset2'

#for root, dirnames, filenames in os.walk(dir):
#    for filename in fnmatch.filter(filenames, '*.txt'):
#        nums.append (re.findall(r'\d+',filename))
#        
#        
#        prefix = '{:04d}'.format(int(nums[cnt][0]))
#        print prefix
#        new_filename = filename
#        while new_filename[0].isdigit():
#            new_filename = new_filename[1:]
#        new_filename = prefix + new_filename
#        filename = dir + '/' + filename
#        new_filename = dir + '/' + new_filename
#        #print "renaming %s to %s" % (filename, new_filename)
#
#        os.rename(filename, new_filename)
#            
#        #prefix = '{:02d}'.format(1)
#        #print nums[cnt][0]
#        cnt = cnt + 1



#print nums

#find all txt files in folder rawdata/ (max 100 files!!!!!!)
matches = []
for root, dirnames, filenames in os.walk(dir):
    for filename in fnmatch.filter(filenames, '*.txt'):
            #nums = re.findall(r'\d+',filename)
            #if nums[0] = cnt
        matches.append(os.path.join(root, filename))
#cnt = cnt + 1

#print matches
#        matches.append(currentdir+'/'+os.path.join(root, filename))


# working folder is psaio. the list has to be
# saved into the ISE working folder (one folder up)
thefile = open('configs/rawdatalist_42k.txt', 'w')

for item in matches:
    print>>thefile, item

print 'All filenames listed and saved to configs/rawdatalist_42k.txt.'
