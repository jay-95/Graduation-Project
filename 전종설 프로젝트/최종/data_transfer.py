import serial
import pymysql

ser = serial.Serial('COM5', 9600)

while True:
    if ser.readable():
        people = ser.readline()
        fire = ser.readline()
        CO = ser.readline()
        
        db = pymysql.connect(host='127.0.0.1', user='root', passwd = 'ooh2oh55', db='in one', charset='utf8')
        
        with db:
            cur = db.cursor()
            
            cur.execute("INSERT INTO `in one values` (people, fire, CO) VALUES(%s, %s, %s)", (people[:-2], fire[:-2], CO[:-2]))
            db.commit()