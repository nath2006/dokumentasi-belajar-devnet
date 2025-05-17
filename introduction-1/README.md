# 💬 Chat Realtime Client-Server C++ dengan Winsock

Dokumentasi ini membahas implementasi aplikasi chat real-time antara satu client dan satu server menggunakan bahasa C++ dan pustaka Winsock pada sistem operasi Windows. Materi mencakup pengantar konsep jaringan, protokol komunikasi, arsitektur OSI, port, hingga penjelasan lengkap implementasi kode.

---

## Konsep Dasar Jaringan Komputer

### Apa Itu Protokol?

**Protokol jaringan** adalah sekumpulan aturan yang digunakan perangkat dalam jaringan untuk saling berkomunikasi, mengatur bagaimana data dikemas, dikirim, diterima, dan ditafsirkan.

#### TCP (Transmission Control Protocol)

- Protokol komunikasi yang **andalan dan berorientasi koneksi**.
- Menjamin bahwa data dikirim dan diterima secara **berurutan dan lengkap**.
- Umumnya digunakan untuk aplikasi yang memerlukan keakuratan data seperti:
  - Web browser (HTTP/HTTPS)
  - Email (SMTP/IMAP/POP3)
  - Aplikasi chat
-  **Digunakan dalam implementasi program ini**.

####  UDP (User Datagram Protocol)

- Protokol komunikasi **tanpa koneksi (connectionless)**.
- Tidak menjamin data sampai secara utuh atau berurutan.
- Keunggulan: ringan, cepat, dan efisien.
- Cocok untuk aplikasi yang membutuhkan kecepatan tinggi:
  - Streaming video/audio
  - Game online
  - DNS Query

---

##  Model Referensi OSI (Open Systems Interconnection)

Model OSI adalah kerangka kerja konseptual yang membagi proses komunikasi jaringan menjadi **7 lapisan terpisah**. Setiap lapisan memiliki fungsi tertentu dalam proses pengiriman dan penerimaan data.

| Lapisan | Nama OSI        | Fungsi Utama                                                      | Contoh Implementasi                        |
|---------|------------------|--------------------------------------------------------------------|--------------------------------------------|
| 7       | **Application**   | Berinteraksi langsung dengan aplikasi pengguna                    | HTTP, FTP, SMTP                             |
| 6       | **Presentation**  | Format data, enkripsi, kompresi                                  | SSL/TLS, JPEG, ASCII                        |
| 5       | **Session**       | Mengatur sesi koneksi dan sinkronisasi                           | API session, NetBIOS                        |
| 4       | **Transport**     | Pemecahan data dan pengiriman ulang bila terjadi kesalahan       | TCP, UDP                                    |
| 3       | **Network**       | Penentuan jalur (routing) dan pengalamatan logis (IP)            | IP, ICMP                                    |
| 2       | **Data Link**     | Pengalamatan fisik dan kontrol kesalahan lokal                   | Ethernet, MAC Address, ARP                  |
| 1       | **Physical**      | Transmisi bit mentah melalui media fisik                         | Kabel, sinyal listrik, konektor             |

> 📌 **Program ini beroperasi pada Lapisan 7 (Application) dan Lapisan 4 (Transport).**

---

##  Konsep Port dalam Jaringan

**Port** adalah nomor logis yang digunakan untuk membedakan berbagai layanan atau aplikasi pada sebuah perangkat jaringan. Setiap koneksi TCP/UDP memiliki kombinasi:
- Alamat IP
- Nomor Port

### Contoh Umum:
| Layanan        | Port Standar |
|----------------|--------------|
| HTTP           | 80           |
| HTTPS          | 443          |
| FTP            | 21           |
| SMTP (Email)   | 25           |
| Custom Chat    | 8080 _(digunakan dalam program ini)_ |

---

##  Konsep Thread (Multithreading)

**Thread** adalah satuan eksekusi terkecil dalam sebuah proses. Dengan multithreading:
- Program dapat menjalankan beberapa tugas secara paralel
- Dalam server chat, satu thread bisa digunakan untuk setiap client agar komunikasi tetap berjalan tanpa saling menunggu

> Dalam implementasi ini, **hanya satu client yang dilayani pada satu waktu**. Untuk melayani banyak client, perlu digunakan **thread atau asynchronous I/O**.

---

## Implementasi Chat TCP di C++ (Windows)

### Struktur File
* chat-realtime<br>
├── server.cpp // Kode server<br>
└── client.cpp // Kode client

---

##  Kompiliasasi

Untuk menjalankan program ini di Windows:

1. **Kompilasi**:
   ```bash
   g++ server.cpp -o server.exe -lws2_32
   g++ client.cpp -o client.exe -lws2_32
2. **Eksekusi**:
    Jalankan ``server.exe`` terlebih dahulu<br>
    Lalu jalankan ``client.exe`` pada jendela terminal lain

3. **Gunakan** ``exit`` untuk mengakhiri sesi dari client atau server


---

## Penjelasan Kode `client.cpp`

```cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
```

- Sama seperti pada server, bagian ini memuat pustaka dan namespace yang dibutuhkan.

```cpp
int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
```

- Inisialisasi Winsock dengan `WSAStartup`.

```cpp
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
```

- Membuat socket TCP untuk client.

```cpp
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
```

- Menyiapkan alamat tujuan (server):
  - `127.0.0.1` adalah localhost.
  - Port `8080` harus sama dengan yang digunakan oleh server.

```cpp
    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    cout << "Terhubung ke server.
";
```

- `connect`: Menghubungkan ke server.
- Jika sukses, tampilkan pesan.

```cpp
    char buffer[1024];
    string message;
```

- Menyiapkan buffer dan variabel string.

```cpp
    while (true) {
        cout << "Client: ";
        getline(cin, message);
        send(clientSocket, message.c_str(), message.length(), 0);
        if (message == "exit") break;
```

- Mengambil pesan dari pengguna, lalu mengirimkannya ke server.
- Jika mengetik `"exit"`, maka keluar dari loop.

```cpp
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        if (strcmp(buffer, "exit") == 0) break;

        cout << "Server: " << buffer << endl;
    }
```

- Menerima respon dari server.
- Jika pesan `"exit"` diterima, client keluar dari loop.

```cpp
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
```

- Menutup socket dan membersihkan resource Winsock.

---
