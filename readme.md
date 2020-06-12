---
Date: 2.6.2020
---

# Update Database Sqlite for GUI

## Yêu cầu

- Chạy với broker localhost
- Cấu trúc json add, remove, Update
- Dùng thư viện của qt

- Lựa chọn checksum để kiểm tra tính toàn vẹn thông tin.

## Project architecture

### main.cpp



## Protocol

### List ID

```json
[
  {"id": "<id>", "ver": <version>},
  {"id": "<id>", "ver": <version>},
  ...
]
```

### Change

Old:

```json
[
  {
    "command":"add/update",
    "id":"<id>",
    "ver": <version>,
    "fullName": "<Full Name>",
    "avt": "<base64 string avt>"
  },
  {
    "command":"remove",
    "id":"<id>",
    "ver": <version>
  }
]
```

New:

```json
{
  "remove": ["<id>", ...],
  "add":
  [
    {
      "id":"(string)<id>",
      "fullName": "(string)<Full Name>",
      "employeeID": "(string)<employeeID>",
      "department": "(string)<department>",
      "level": (string)<level>,
      "avt": "(base64 string)<avt>",
      "ver": (int)<version>,
    },
    ...
  ],
  "update":
  [
    {
      "id":"(string)<id>",
      "fullName": "(string)<Full Name>",
      "employeeID": "(string)<employeeID>",
      "department": "(string)<department>",
      "level": (string)<level>,
      "avt": "(base64 string)<avt>",
      "ver": (int)<version>,
    },
    ...
  ]
}
```