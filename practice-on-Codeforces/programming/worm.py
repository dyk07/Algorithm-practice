#!/usr/bin/python
# coding: UTF-8


from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import Select, WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

class THUSpaceBooking:
    def __init__(self, username, password):
        self.username = username
        self.password = password
        self.driver = None
        self.base_url = "https://m.myhome.tsinghua.edu.cn/"
        
    def login(self):
        """登录系统"""
        self.driver = webdriver.Chrome()  # 需要安装ChromeDriver
        self.driver.get(self.base_url + "login.aspx")  # 假设登录页面
        
        # 输入用户名密码（需要根据实际登录页面调整）
        username_input = self.driver.find_element(By.NAME, "username")  # 根据实际调整
        password_input = self.driver.find_element(By.NAME, "password")  # 根据实际调整
        
        username_input.send_keys(self.username)
        password_input.send_keys(self.password)
        
        # 提交登录
        login_button = self.driver.find_element(By.ID, "loginButton")  # 根据实际调整
        login_button.click()
        
        time.sleep(3)
        
    def book_space(self):
        """预约空间"""
        # 访问预约页面
        self.driver.get(self.base_url + "kj_yuyue.aspx?xieyi=1")
        time.sleep(2)
        
        # 空间已默认为"31号楼社区活动中心"，不需要更改
        
        # 选择B1钢琴房
        piano_radio = self.driver.find_element(By.ID, "kj_yuyueCtrl1_RadioButtonList2_0")
        piano_radio.click()
        time.sleep(1)
        
        # 选择日期2026-01-18
        # 先点击其他日期触发页面刷新，再点击目标日期
        date_13 = self.driver.find_element(By.ID, "kj_yuyueCtrl1_RadioButtonList3_1")
        date_13.click()
        time.sleep(2)
        
        # 找到并点击2026-01-18
        date_elements = self.driver.find_elements(By.XPATH, "//input[@name='kj_yuyueCtrl1$RadioButtonList3']")
        for date_element in date_elements:
            if date_element.get_attribute("value") == "2026-01-18":
                date_element.click()
                break
        time.sleep(2)
        
        # 查找16:00-17:00时间段
        # 由于页面通过AJAX加载，需要等待
        WebDriverWait(self.driver, 10).until(
            EC.presence_of_element_located((By.ID, "kj_yuyueCtrl1_Repeater1_ctl06_lbltime"))
        )
        
        # 查找16:00对应的行
        rows = self.driver.find_elements(By.XPATH, "//tr[contains(@id, 'kj_yuyueCtrl1_Repeater1_ctl')]")
        for row in rows:
            try:
                time_element = row.find_element(By.XPATH, ".//span[contains(@id, 'lbltime')]")
                if "16:00" in time_element.text:
                    # 检查是否可预约
                    status_element = row.find_element(By.XPATH, ".//span[contains(@id, 'lblstate')]")
                    if "已被预约" not in status_element.text:
                        # 找到预约按钮并点击
                        book_button = row.find_element(By.XPATH, ".//input[@type='button' or @type='submit']")
                        book_button.click()
                        print("预约成功！")
                        return True
                    else:
                        print("该时间段已被预约")
            except:
                continue
                
        print("未找到可预约的时间段")
        return False
        
    def run(self):
        """主运行函数"""
        try:
            self.login()
            self.book_space()
            input("按Enter键退出...")
        finally:
            if self.driver:
                self.driver.quit()

# 使用示例
if __name__ == "__main__":
    # 请替换为你的用户名和密码
    booker = THUSpaceBooking("2025010912", "rSCE93WN8CpNn5N")
    booker.run()