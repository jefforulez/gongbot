
# gongbot

Announce gong strikes in IRC

## Introduction

The goal of this project is simple:  Announce nerf-to-gong interactions in IRC.

The [@blip](http://twitter.com/blip) office has several characteristics that made this project a necessity:

1. Gongs.  We have two of them.
2. Nerf guns.  We have way more than two of them.
3. IRC.  We only have one IRC server, but it has lots of users

## Code

This project went through several configurations - hardware and software - before
I settled on the following setup:

#### Sensor

* Piezo transducer -> Arduino -> Ethernet shield
* `./gong_sensor_ethernet`

#### IRC Gateway

* Node.js app running on my desktop iMac
* `./gong_bot_js`


## Crappy Code

There's code in here for other working, rubegoldbergesque, configurations:

#### Xbee-to-Ethernet

* Sensor
  * Pieze transducer -> Arduino -> Xbee
  * `./gong_sensor`

* Receiver
  * Xbee -> Arduino -> Ethernet Shield
  * `./gong_receiver`

* IRC Gateway
  * Perl script that listens for gong events on a TCP/IP socket
  * `./gong_bot`

#### Xbee-to-Serial

* Sensor
  * Pieze transducer -> Arduino -> Xbee
  * `./gong_sensor`

* Receiver
  * Xbee -> Arduino -> Serial USB
  * `./gong_receiver_serial`

* IRC Gateway
  * Perl script that listens for gong events on serial /dev
  * `./gong_bot`


