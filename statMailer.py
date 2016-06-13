import smtplib
import socket
import traceback
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
from email.MIMEBase import MIMEBase
from email import encoders

fromaddr = 'ydornbergtest@gmail.com' # email to send email from
toaddr = 'ydornber@uci.edu' #email to send to

msg = MIMEMultipart()

msg['From'] = fromaddr
msg['To'] = toaddr
msg['Subject'] = "Platformer Stats"

body = "Attached is a .txt file containing the stats from your gameplay. Enjoy!"

msg.attach(MIMEText(body,'plain'))

filename = "platformStats.txt"
attachment = open("platformStats.txt", "rb")

part = MIMEBase('application','octet-stream')
part.set_payload((attachment).read())
encoders.encode_base64(part)
part.add_header('Content-Disposition',"attachment; filename= %s" % filename)

msg.attach(part)
tries = 0
while True:
    if (tries >60):
        exit()
    print str(tries) + "\n"
    try:
        smtpserver = smtplib.SMTP('smtp.gmail.com',587, timeout=30)
        break
    except EXCEPTION as e:
        tries = tries + 1
        print e.__doc__
        print e.message
        time.sleep(1)

smtpserver.ehlo()
smtpserver.starttls()
smtpserver.ehlo()
smtpserver.login(fromaddr, 'raspberrypi')
text = msg.as_string()
smtpserver.sendmail(fromaddr, toaddr, text)
smtpserver.quit()
