# SSH iPhone Respring Tool

เครื่องมือสำหรับรีสปริง iPhone ผ่าน SSH (สำหรับเครื่องที่ jailbreak แล้ว)  
โปรแกรมนี้ใช้สำหรับส่งคำสั่ง `killall -9 SpringBoard` ไปยังอุปกรณ์ iOS ผ่านการเชื่อมต่อ SSH เพื่อทำการรีสปริงเครื่อง

---

## ข้อควรระวัง
- **คำสั่งนี้จะทำให้ SpringBoard รีสตาร์ททันที** ใช้เฉพาะเมื่อจำเป็นเท่านั้น
- ใช้กับเครื่องที่ jailbreak แล้วและติดตั้ง OpenSSH ไว้เท่านั้น
- ควรเปลี่ยนรหัสผ่าน default ของผู้ใช้ `mobile` บนเครื่อง iOS เพื่อความปลอดภัย

---

## ข้อกำหนด
- **libssh**: ต้องติดตั้งไลบรารี่ `libssh` ก่อนคอมไพล์
- **รหัสผ่าน SSH**: ต้องตั้งค่า environment variable `SSH_PASS` ก่อนรันโปรแกรม

---

## วิธีติดตั้ง

### ติดตั้ง libssh
บนระบบ Ubuntu/Debian:
```bash
sudo apt-get install libssh-dev
```

### บนระบบ macOS (ใช้ Homebrew):
```bash
brew install libssh
```
### วิธีคอมไพล์
ใช้คำสั่งต่อไปนี้เพื่อคอมไพล์โปรแกรม
```bash
gcc ssh-iphone-respring.c -o ssh-iphone-respring -lssh
```
## วิธีใช้งาน
1. ตั้งค่ารหัสผ่าน
ตั้งค่า environment variable SSH_PASS ด้วยรหัสผ่าน SSH ของเครื่อง iOS:
```bash
export SSH_PASS="your_ssh_password"
```
## 2. รันโปรแกรม
รันโปรแกรมด้วยคำสั่ง:
```bash
./ssh-iphone-respring
```
# ข้อควรรู้
SpringBoard: เป็นโปรเซสหลักที่จัดการหน้าจอและ UI ของ iOS

รีสปริง: การรีสตาร์ท SpringBoard ทำให้การเปลี่ยนแปลงการตั้งค่าหรือธีมมีผลทันที


