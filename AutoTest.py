'''
@Date: 2020-06-17 14:09:42
@LastEditors: zyk
@LastEditTime: 2020-06-17 15:40:36
@FilePath: /compiler/AutoTest.py
'''

# TODO: 目前只能检查编译程序是否异常退出和打印编译器输出，后续增加输入输出比较模块

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

args = parser.parse_args()

# get all test imput file name
test_file = list(filter(lambda x: re.fullmatch(args.name, x) != None, os.listdir(args.dir)))
if args.verbose:
    print("Test Input: ", test_file)

if args.dir[-1] != '/':
    test_dir = args.dir + '/'
else:
    test_dir = args.dir

# run test and check if the compiler returns 0
error_count = 0
for file in test_file:
    if args.verbose:
        print("Compiling", file)
    try:
        out = subprocess.check_output(['./compiler', test_dir + file], stderr=subprocess.STDOUT).decode('utf-8')
        if args.verbose:
            print(out)
    except subprocess.CalledProcessError as e:
        error_count += 1

print(len(test_file), "file(s) tested,", error_count, "errors occurred.")
    
