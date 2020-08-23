import os
import re
import argparse
import subprocess

parser = argparse.ArgumentParser(description="Test our compiler automatically.")

# set the path of test input dir
parser.add_argument("-d", "--dir", help="set the dir path of test input", required=True)

source = parser.add_mutually_exclusive_group(required=True) # test source file
# set regex of file name
source.add_argument("-n", "--name", help="regex to filt test file", default="[0-9A-Za-z_]*.sy")
# run all function test and check if they all return zero
source.add_argument("-a", "--all", help="run all test under the dir", action="store_true")

# show compiler output
parser.add_argument("-v", "--verbose", help="show all output of compiler", action="store_true")

# stop before compile each file
parser.add_argument("-c", "--confirm", help="require confirmation before processing each file", action="store_true")

args = parser.parse_args()

# get all test imput file name
test_file = list(filter(lambda x: re.fullmatch(args.name, x) != None, os.listdir(args.dir)))
if args.verbose:
    print("Test Input:\n" + '\n'.join(test_file))

if args.dir[-1] != '/':
    test_dir = args.dir + '/'
else:
    test_dir = args.dir

# run test and check if the compiler returns 0
error_count = 0
error_filenames = list() # save file names with errors

for file in test_file:
    if args.confirm:
        conf = input("Compile next file [ " + file + " ] ? (y/n)")
        if conf != "y":
            exit()

    if args.verbose:
        print("Compiling", file)
    try:
        out = subprocess.check_output(['./compiler', test_dir + file], stderr=subprocess.STDOUT)
        if args.verbose:
            try:
                print(out.decode('utf-8'))
            except UnicodeDecodeError:
                print("\u001b[31mERROR!\u001b[0m cannot decode as utf-8 character!")
            
    except subprocess.CalledProcessError:
        error_count += 1
        error_filenames.append(file)

print(len(test_file), "file(s) tested,", error_count, "error(s) occurred.")
if (error_count > 0):
    print("\nPlease check these files:\n")
    print('\n'.join(error_filenames))
