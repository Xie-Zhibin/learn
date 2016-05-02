#-*-coding:utf-8-*-
from email.header import Header
from email.mime.text import MIMEText
import smtplib

from_addr = "youremail@example.com" # 发送方邮箱
email_password = "xxxxxx"  # 未开启客户端授权密码的163邮箱，可以直接用登录密码，其他未试
smtp_server = "smtp.example.com"  # smtp服务器地址,如smtp.163.com
text = u""  # 正文内容


def sent_email(emails):
    to_addr = emails  # 用户的邮箱地址，列表
    msg = MIMEText(text, 'plain', 'utf-8')  # 构造MIMEText对象
    msg['Subject'] = Header(u'', 'utf-8')  # 邮件主题
    msg['From'] = "Example <%s>" % from_addr  # 邮件发送方
    msg["To"] = "You <Your_Email>"  # 邮件接收方

    server = smtplib.SMTP(smtp_server, 25)
    server.starttls()  # 建立安全连接，QQ, Gmail必须加密传输
    server.login(from_addr, email_password)  # 登录smtp服务器
    server.sendmail(from_addr, to_addr, msg.as_string())  # 发送邮件
    server.quit()  # 登出服务器
    return "finish"
