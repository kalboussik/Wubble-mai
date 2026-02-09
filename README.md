┌─────────────────────────────── PC / Host ───────────────────────────────┐
│                                                                          │
│  Navigateur (UI Web)                                                     │
│  ──────────────────── HTTP/WS ────────────────────┐                      │
│                                                     │                    │
│                         ┌──────────── Docker ─────────────┐             │
│                         │                                  │             │
│                         │   app.py (Flask + SocketIO)      │             │
│                         │   - API /api/connect             │             │
│                         │   - WS: push logs & status       │             │
│                         │   - Serial worker (pyserial)     │             │
│                         │                                  │             │
│                         └───────────────┬──────────────────┘             │
│                                         │  pyserial                      │
│                                         │  rfc2217://...  ou socket://...│
│                                         ▼                                │
│                         ┌──────────────────────────────────┐             │
│                         │ Bridge TCP ↔ Serial               │             │
│                         │ - RFC2217 server (optionnel)      │             │
│                         │ - ou TCP raw (socat/ser2net raw)  │             │
│                         └──────────────────┬───────────────┘             │
│                                            │ USB                          │
└────────────────────────────────────────────┼─────────────────────────────┘
                                             │
                                             ▼
                                  ┌───────────────────┐
                                  │   ESP32-S3         │
                                  │  USB Serial JTAG   │
                                  │  (ou UART0 selon)  │
                                  │                   │
                                  │  app_control task  │
                                  │  - lit JSON lines  │
                                  │  - set/get params  │
                                  │  - run tests       │
                                  │                   │
                                  │  node_task / BLE   │
                                  └───────────────────┘
