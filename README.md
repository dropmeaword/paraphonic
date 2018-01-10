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
|---|---|---|
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

#### How to give the ptive portal another domain name

Because we run our own DNS in the Rpi you can give the website any DNS name entry we like, the default hand configured ones are `paraphonic.com` and `polydiso.com` but you can call it anything you like.

You just have to add an entry to this file `/etc/paraphonic.hosts`.

