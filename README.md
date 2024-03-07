# Setup Programs
## Instruction : Setup
```shell
apt-get update
apt upgrade
apt install net-tools
apt install build-essential
apt-get install manpages-dev
apt install nano
apt install gpiod
apt install htop
apt install bridge-utils
apt install cron
apt install ntpdate
apt-get install libmodbus-dev
sudo apt-get install libncurses5-dev libncursesw5-dev
```

# System Setting
## Set Network Properties Path
- Run command in commandline below,
```shell
nano /etc/network/interfaces.d/<interface>
```
### Interface: eth0
- Description: Set WAN to LAN and assign ip to eth0
```sh
# eth0.txt

auto eth0
iface eth0 inet static
    address 192.168.2.8
    netmask 255.255.255.0
    broadcast 192.168.2.255
```
### Interface: eth1
- Description : Assign ip to eth1
```sh
# eth1.txt

auto eth1
iface eth1 inet static
    address 192.168.11.8
    netmask 255.255.255.0
    broadcast 192.168.11.255
```
## Set Network Bridge
- Run command in commandline below,
```shell
nano /etc/sysctl.conf
```
Uncomment line
```sh
--> # net.ipv4.ip_forward=1
```
- Assign IP to bridge br0
```shell
nano /etc/network/interfaces.d/br0
```
```sh
# br0.txt

auto eth0
	iface eth0 inet manual
auto eth1
	iface eth1 inet manual
auto br0
	iface br0 inet static
	address 192.168.12.49
    netmask 255.255.255.0
	gateway 192.168.12.255
	bridge_ports eth0 eth1
	bridge_stp off
	bridge_fd 0
```
## Apply setting
```shell
sudo service networking restart
```

# GPIO Setup
```shell
echo <pin_number> > /sys/class/gpio/export
```
Pin Number calculation: GPIO2_A3 --> GPIO(X)_(Y**)(Z)
</p> Formula: <pin_number> = X*32 + Y*8 + Z, 
Y** --> A =0, B=1, C=2 ... </p>

```shell
echo "mode" > /sys/class/gpio/gpio<pin_number>/direction
```
"mode" --> "in", "out"
```shell
echo <value> > /sys/class/gpio/gpio<pin_number>/value
```
< value> --> 1 = HIGH, 0 = LOW

# NTP Service Setup
## Set timezone
```shell
timedatectl list-timezones
sudo timedatectl set-timezone Asia/Bangkok
```

## Synchronize time with NTP server
```shell
ntpdate <NTP Server>
```

## etc.
- Systemd-timesyncd
    1. apt install systemd-timesyncd
    2. nano /etc/systemd/timesyncd.conf
    3. systemctl restart systemd-timesyncd

- CHRONY
    1. apt install chrony
    2. nano /etc/chrony/chrony.conf
    3. pool <YOUR NTP SERVER> iburst

# Remote Configuration (TMConfiguration)
## Turn on bridge network for configuration
- Run command below,
```shell
nano br_On.sh
```
br_On.sh

    #!/bin/bash
    # Crete bridge
    brctl addbr br0

    # Add existing interfaces to bridge
    brctl addif br0 eth1 eth2

    # Or assign IP address to bridge interface (Don't recommend)
    ip addr add 192.168.11.10/24 brd + dev br0 // ip route add default via 192.168.2.1 dev br0

## Turn off bridge network when finish configuring
- Run command below,
```shell
nano br_Off.sh
```
br_Off.sh

    #!/bin/sh
    ip link set br0 down
    brctl delbr br0

Run command below to activate
```shell
chmod 774 br_Off.sh
```

# Kill the RVD program when its perform background runs
- Open system monitoring tool
```shell
htop
```
- Find ".rvd" process
- Press "F9" -> SIGKILL
- Press "F10" to quit

# Executing the RVD Program
Run command below
```shell
chmod +x run.sh
./run.sh
```
and it will ask for Radar IP Address
```shell
Enter Radar IP Address: <Radar_IP_Address>
```
# Startup
- Go to rc.local
```shell
nano /etc/rc.local
```

rc.local

    #!/bin/sh -e
    # rc.local

    # List of programs to run at startup
    /root/bridge_off.sh &
    /root/RVD_APP/run.sh &

# Task scheduler
Run 
```shell
crontab -e
```

    SHELL=/bin/sh
    HOME=/root
    PATH=/usr/local/sbin:usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

    # * * * * * your_command
    # | | | | |
    # | | | | +-- Day of the week (0 - 7) (Sunday is 0 or 7)
    # | | | +---- Month (1 - 12)
    # | | +------ Day of the month (1 - 31)
    # | +-------- Hour (0 - 23)
    # +---------- Minute (0 - 59)

    5 * * * * /root/bridge_off.sh # Turn off bridge network every 5 minutes 
    * * * * * /root/keep_alive.sh # Check the RVD program every 1 minute 
</p>

    SHELL=/bin/bash
    HOME=/home/apo
    PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

    * * * * * /bin/bash /home/apo/restart_script.sh
    service cron restart or systemctl restart cron

Instruction "usbipd wsl attach --busid <id>
