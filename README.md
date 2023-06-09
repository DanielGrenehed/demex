# DEMEX
ESP32 DMX512 implementation through a MAX485

## DMX512

### Packet
A normal DMX512 packet consists of 513 frames, 512 frames for controlling the universe and a start frame.
A start frame of 0x00 refers to a standard light control packet. DMX 'addresses' defines where in the packet a device is getting its control value from.
   
| frame number | purpose |
| ------------ | ------- |
| 0 | start frame |
| 1-513 | control values |

### Start Codes

| Start code | Purpose |
| ---------- | ------- |
| 0x00 | start of light control |
| 0x17 | ASCII Text Packet |
| 0x55 | Test Packet | 
| 0x90 | UTF-8 Text Packet |
| 0x91 | Manufacturer specific use |
| 0x92 - 0xA9 | Possible future revisions of standard. Use not permitted |
| 0xAb - 0xCD | Possible future revisions of standard. Use not permitted |
| 0xCF | System Information Packet |
| 0xF0 - 0xF7 | Prototyping, Not permitted in shipped product | 

