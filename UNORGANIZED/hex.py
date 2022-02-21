# ----------CALIBRATION----------
# #Set Calibration Value
# DF0 = 0x02
# DF1 = 0x58
# CS  = 0x8F

# command = [0x11, 0x03, 0x03, DF0, DF1, CS]


# ppm = DF0*256+DF1
# #print(ppm)

# cs = hex(256 - (command[0]+command[1]+command[2]+command[3]+command[4])%256)
# #print(cs)



# def calibrate(ppm):
#     DF0 = ppm//256
#     DF1 = ppm - DF0*256
#     print('DF0:', DF0, hex(DF0))
#     print('DF1:', DF1, hex(DF1))
#     print('ppm:', DF0*256+DF1)

#     CS = hex(256 - (0x11 + 0x03 + 0x03 + DF0 + DF1)%256)
#     command = [hex(0x11), hex(0x03), hex(0x03), hex(DF0), hex(DF1), CS]
#     print(command)

# calibrate(600)

# ----------SOFTWARE----------
# #def software()
# #    SN1 = 0x00
# #    SN2 = 0x00
# #    SN3 = 0x
# #    SN4
# #    SN5


# SN1 = 0x00 << 8 | 0x00
# SN2 = 0x02 << 8 | 0x11
# SN3 = 0x03 << 8 | 0x8C
# SN4 = 0x0A << 8 | 0xD3
# SN5 = 0x00 << 8 | 0x08

# SN_list = [SN1, SN2, SN3, SN4, SN5]
# print(["{:04d}".format(i) for i in SN_list])
# #print((round(SN1,4) SN2, SN3, SN4, SN5)
# #print("{:04d}".format(SN1))
# ##print("{:04d}".format(SN2))
# #print("{:04d}".format(SN3))
# #print("{:04d}".format(SN4))
# #print("{:04d}".format(SN5))


# array = [0x11, 0x02, 0x51, 0x01] #set working status command
# array = [0x16, 0x01, 0x51] #set working status response
# array = [0x11, 0x01, 0x51] #check working status command
# cs = hex(256 - (sum(array))%256)
# print(cs)


# ----------MEASUREMENT ----------
def func_period(period, smooth):
    DF1 = period//256
    DF2 = period - DF1*256
    #print('DF1:', DF1, hex(DF1))
    #print('DF2:', DF2, hex(DF2))
    #print('period:', DF1*256+DF2)

    CS = hex(256 - (0x11 + 0x04 + 0x50 + DF1 + DF2 + smooth)%256)
    command = [hex(0x11), hex(0x04), hex(0x50), hex(DF1), hex(DF2), hex(smooth), CS]
    print(command)
    

func_period(120,15)
func_period(2,2)
func_period(2,4)
func_period(10, 4)
func_period(4, 2)
#period = DF1*256+DF2


#0x11, 0x04, 0x50, DF1, DF2, DF3, CS

#array = [0x11, 0x04, 0x50, 0x00, 0x78, 0x0F]
#cs = hex(256 - (sum(array))%256)
#print(cs)