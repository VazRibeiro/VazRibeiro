# WireGuard Setup

Example configuration for WireGuard on Ubuntu 22.04.

```
[Interface]
PrivateKey = <your private key>
Address = 10.0.0.1/24
ListenPort = 51820

[Peer]
PublicKey = <peer public key>
AllowedIPs = 10.0.0.2/32
Endpoint = example.com:51820
```
