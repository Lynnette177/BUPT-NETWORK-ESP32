import requests
import time

BUPTACC = "ACC"
BUPTPASS = "PASS"

BAIDU_HOST = "http://www.baidu.com/"
REDIRECT_URL = "http://www.baidu.com/?cmd=redirect&arubalp=12345"
LOGIN_URL = "http://10.3.8.216/login"
HEADERS = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0",
    "Content-Type": "application/x-www-form-urlencoded"
}

def bupt_login(BUPTACC, BUPTPASS):
    time.sleep(0.5)
    postData = f"user={BUPTACC}&pass={BUPTPASS}"
    response = requests.get(REDIRECT_URL, timeout=100)
    print(response.status_code)
    print(response.headers)

    cookie_user = response.cookies.get_dict()
    print(cookie_user)

    response = requests.post(LOGIN_URL, headers=HEADERS, data=postData, cookies=cookie_user)

    if response.status_code > 0:
        print(f"HTTP Response code: {response.status_code}")
        print("Response: ", response.status_code)
    else:
        print(f"HTTP Request failed. Error code: {response.status_code}")
        print(response.reason)

    time.sleep(1)


if __name__ == "__main__":
    bupt_login(BUPTACC, BUPTPASS)
