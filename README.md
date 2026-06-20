Hiện có 3 âm thanh: Default, Success, Failed

Gửi đến ESP8266 tín hiệu dạng: Mã|Nội dung hiển thị
Ví dụ: 300|Hehehe thì sẽ hiển thị Hehehe kèm âm thanh Success.

Có thể gửi POST để hiển thị nội dung. Sử dụng mã 103 để xem địa chỉ IP. Lệnh POST có dạng: http://192.168.1.14/command?command=300|hehehe, trong đó `300|hehehe` là nội dung gửi. 

# Bảng mã và nội dung hiển thị

| Mã | Hiển thị | Âm thanh |
| - | - | - |
| 100 | LuToTahSo |  |
| 101 | Hello World! |  |
| 102 | Config Wifi |  |
| 103 | Wifi Connected |  |
| 104 | Wifi Status |  |
| 200 | Build Success | Success |
| 201 | Build Failed | Failed |
| 300 |  | Success |
| 301 |  | Failed |
<!-- |  |  | -->

