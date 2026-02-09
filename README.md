flowchart TB

    %% UI Layer
    UI[Web Browser<br/>UI Dashboard]

    %% Docker Backend
    subgraph Docker["Docker Container"]
        APP[app.py<br/>(Flask + Socket.IO)]
        SERIAL[Serial Worker<br/>(pyserial)]
        APP --> SERIAL
    end

    %% Serial Bridge
    BRIDGE[Serial-to-TCP Bridge<br/>(RFC2217 / Raw TCP<br/>socat / ser2net)]

    %% ESP32
    subgraph ESP32["ESP32-S3"]
        USB[USB Serial JTAG / UART]

        CONTROL[app_control task<br/>JSON Lines Protocol<br/>set / get / run]
        NODE[node_task<br/>BLE (NimBLE)<br/>SX1261 / WuR Tests]
        PARAMS[params module<br/>Runtime configuration]

        USB --> CONTROL
        CONTROL --> NODE
        CONTROL --> PARAMS
        PARAMS --> NODE
    end

    %% Connections
    UI <-->|HTTP / WebSocket| APP
    SERIAL <-->|TCP| BRIDGE
    BRIDGE <-->|USB| USB
