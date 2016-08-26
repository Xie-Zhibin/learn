"""Emaill demo."""

from email.header import Header
from email.mime.text import MIMEText
import smtplib


def send_email(text, to_addr):
    """Send Email, arg:'to_addr' is a list."""
    from_addr = 'youremail'  # e.g.: 1234@163.com
    email_password = 'password'
    smtp_server = 'smtp.163.com'
    msg = MIMEText(text, 'html', 'utf-8')  # structure MIMEText object
    msg['Subject'] = Header(u'Email head.', 'utf-8')
    msg['From'] = "island<%s>" % from_addr
    msg["To"] = "Dear User %s" % to_addr[0]
    server = smtplib.SMTP(smtp_server, 25)
    # set up secure connection，QQmail and Gmail must encrypt to transport
    server.starttls()
    server.login(from_addr, email_password)  # login smtp server
    server.sendmail(from_addr, to_addr, msg.as_string())  # send maill
    server.quit()  # logout server
    return {
        'msg': 'done'
    }
