## PARAPHONIC

Code for the PARA-PHONIC POLY-DISO a work by Remco van Bladel.

Technical development: Luis Rodil-Fernandez & Charlie Berendsen

## Accessing the Access Point

#### For administration

The access point should be available in IP address `192.168.42.1`. You can access it in the following ways.

###### SSH

You can `ssh someone@192.168.42.1` and if your public SSH key is installed you should be able to log in straight away.

###### Web control panel

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