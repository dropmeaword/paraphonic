## PARAPHONIC

Code for the PARA-PHONIC POLY-DISO a work by Remco van Bladel.

Technical development: Luis Rodil-Fernandez & Charlie Berendsen

## Devices and addresses

The IP addresses of these devices are invariable, they will always stay the same.

|device|address|function|
|---|---|---|
|raspberry pi|192.168.42.1|runs access point and captive portal webserver|
|wemos (hardware controller)|192.168.42.101|runs a tiny webserver that allows you to directly control light + motors|

#### Accessing the Access Point

###### For administration

The access point should be available in IP address `192.168.42.1`. You can access it in the following ways.

###### SSH

You can `ssh someone@192.168.42.1` and if your public SSH key is installed you should be able to log in straight away.

### Interfacing with the hardware

You can communicate with the hardware controller using HTTP `GET` requests, like you would do to interface with other web services.

The base URL for all your requests to the hardware will be `http://192.168.42.101`, to this URL you can add the following endpoints to interface with the different elements:

|endpoint|accepted parameters|function|
|---|---|---|---|
|/spin|*id*: motor id, *state*: on/off, *time*: unning time in millis|spins the motor with *id* for *milliseconds*, the *state* parameter is ignored in the current implementation.
|/light/test|n.a.|flashes an R, G ,B sequence on the lights, this is used for testing purposes|
|/light/solid|*solid*: css color code (without the #)|sets all lights to the color expressed in the color code|

##### Examples

|GET request|effect|
|---|---|
|http://192.168.42.101/spin?id=1&time=15000|spins motor 1 for 15 seconds|
|http://192.168.42.101/spin?id=2&time=3000|spins motor 2 for 3 seconds|
|http://192.168.42.101/light/solid?solid=ffff00|sets all LEDs to full-saturation yellow|
|http://192.168.42.101/light/solid?solid=303030|sets all LEDs to a dimmed white|

## How to do things

#### How to change the SSID (name of the wifi network)

The Rpi runs a process called `hostapd` that is basically what turns the Rpi into an access point (AP). `hostapd` loads a confiuration file named  `/etc/hostapd.conf`, to change the name of the access point you have to edit that file, like this (you must become root or run all commands with `sudo`):

First we backup the original file, just in case:
```
# cp /etc/hostapd.conf /etc/hostapd.config.original
```

Then we open the file with a text editor, like `nano`:

```
# nano /etc/hostapd.conf
```

Change the line that shows `ssid=PARAPHONIC` and change the word *PARAPHONIC* for whatever name you want to give the AP. Save the config file and leave the editor, after this you will have to restart the `hostapd` service, like this:

```
# service hostapd restart
```

If everything went well you should see no output the `service` command only produces output when something goes wrong.

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
