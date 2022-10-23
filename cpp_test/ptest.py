fd = open("/dev/test_file", 'w')
s = input("please enter Data:")
fd.write(s)
# fd.close ()

fd = open("/dev/test_file", 'r')
print(fd.readline())
fd.close()
