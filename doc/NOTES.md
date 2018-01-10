# Dev notes

## Web control panel

The Rpi AP runs a simple web-based configuration panel, which you can find at [http://192.168.42.1:8080](http://192.168.42.1:8080) ask for login credentials.


#### For website deployment

_See SSH access above_

#### As member of the audience

## DHCP configuration on the Rpi wifi AP

The `dnsmasq` config takes case of DNS and DHCP services for the wifi access point in the Rpi, dnsmas reads it's config from `/etc/dnsmasq.conf` of from file located in `/etc/dnsmasq.d/`. I configured to _static dhcp_ leases so the hosts in the installation (atlantic and wemos) all have fixed IPs.

```
interface=wlan0
dhcp-range=192.168.42.2,192.168.42.30,255.255.255.0,12h

server=8.8.8.8
#address=/#/192.168.42.1
except-interface=eth0

dhcp-host=34:36:3b:cb:3b:ae,192.168.42.100  # atlantic
dhcp-host=60:01:94:82:89:27,192.168.42.101  # wemos
```

The _wemos_ node is also running a tiny mDNS service that gives the gadget a fixed local DNS entry under the name of `pyropanda.local`.

if you ping `pyropanda.local` from the AP you will see:

```
PING pyropanda.local (192.168.42.101) 56(84) bytes of data.
64 bytes from ESP_828927 (192.168.42.101): icmp_seq=1 ttl=128 time=8.36 ms
64 bytes from ESP_828927 (192.168.42.101): icmp_seq=2 ttl=128 time=4.26 ms
64 bytes from ESP_828927 (192.168.42.101): icmp_seq=3 ttl=128 time=4.90 ms
64 bytes from ESP_828927 (192.168.42.101): icmp_seq=4 ttl=128 time=5.81 ms
64 bytes from ESP_828927 (192.168.42.101): icmp_seq=5 ttl=128 time=5.80 ms
```

## FROM 'PARAPHONIC' ACCESS POINT

Executing `iw dev wlan0 station dump` as root, shows these two nodes.

```
Station 34:36:3b:cb:3b:ae (on wlan0)  (workstation)
Station 60:01:94:82:89:27 (on wlan0)  (wemos node)
```

## Example requests

LEDs

```
http://10.0.0.15/light/solid?solid=f0ffee
```

MOTORS

```
http://10.0.0.15/spin?id=1&time=4000
```

#### bad iptables rules

these I am not sure about:

```
iptables -t mangle -N internet iptables -t mangle -A PREROUTING -i wlan0 -p tcp -m tcp --dport 80 -j internet iptables -t mangle -A internet -j MARK --set-mark 99
iptables -t nat -A PREROUTING -i wlan0 -p tcp -m mark --mark 99 -m tcp --dport 80 -j DNAT --to-destination 192.168.10.1
```

these ones didn't do what I want:

```
someone@paradiso:/etc/lighttpd$ sudo iptables -S
-P INPUT ACCEPT
-P FORWARD ACCEPT
-P OUTPUT ACCEPT
-A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
-A FORWARD -i wlan0 -o eth0 -j ACCEPT
someone@paradiso:/etc/lighttpd$ sudo iptables -t nat -A PREROUTING -p tcp --dport 80 -j DNAT --to-destination 192.168.42.1
someone@paradiso:/etc/lighttpd$ sudo iptables -P FORWARD DROP
someone@paradiso:/etc/lighttpd$ sudo iptables -S
-P INPUT ACCEPT
-P FORWARD DROP
-P OUTPUT ACCEPT
-A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
-A FORWARD -i wlan0 -o eth0 -j ACCEPT
```
