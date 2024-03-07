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
    iface eth0 inet static
    address 192.168.2.8
    netmask 255.255.255.0
    broadcast 192.168.11.255
auto eth1
    iface eth1 inet static
    address 192.168.11.8
    netmask 255.255.255.0
    broadcast 192.168.11.255
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

Instruction: NTP Service
{
    Set timezone:
    timedatectl list-timezones
    sudo timedatectl set-timezone Asia/Bangkok

    1. apt install systemd-timesyncd
    2. nano /etc/systemd/timesyncd.conf
    3.systemctl restart systemd-timesyncd

    CHRONY
    1. apt install chrony
    2. nano /etc/chrony/chrony.conf
    3. pool <YOUR NTP SERVER> iburst


    systemctl unmask systemd-timesyncd.service
    systemctl restart systemd-timesyncd

    or

    ntpdate <NTP Server>
}
################################ Bash script ################################

Instruction: "nano br_On.sh"
Description : Turn on bridge network
{
    #!/bin/bash
    # Crete bridge
    brctl addbr br0

    # Assign IP address to bridge interface
    ip addr add 192.168.11.10/24 brd + dev br0 // ip route add default via 192.168.2.1 dev br0

    # Add interfaces to bridge
    brctl addif br0 eth1 eth2
}

Instruction: "nano br_Off.sh"
Description : Turn off bridge network
{
    #!/bin/sh
    ip link set br0 down
    brctl delbr br0
}
RUN "chmod 774 br_Off.sh" to activate

Instruction: "nano RVD_Shutdown.txt"
Description : Kill the RVD program when its perform background runs
{
    Run these in sequence
    {
        1."htop"
        2.Find ".rvd" process
        3.Press "F9" -> SIGKILL
        4.Press "F10"
    }
}

Instruction: "nano keep_alive.sh"
Description : Run the RVD program continueously
{
    #!/bin/bash

    program_name="./rvd 192.168.11.55 60000"
    max_attempts=10
    attempt=1

    # Function to run the program and check its exit status
    run_rvd() {
        echo "Running ./rvd"
        ./rvd 192.168.11.55 60000
        return $?
    }

    if pgrep -f "$program_name" > /dev/null; then
        echo "The RVD Program is running"
    else
        echo "The RVD Program isn't running at this moment"

        while [ $attempt -le $max_attempts ]; do
            echo "Attempt $attempt: Running ./rvd"

            run_rvd
            exit_status=$?

            if [ $exit_status -eq -1 ]; then
                echo "The ./rvd returned -1. Restarting..."
            else
                echo "The ./rvd exit status: $exit_status"
                break
            fi

            ((attempt++))
        done

        if [ $attempt -gt $max_attempts ]; then
            echo "Restarting failed, Reporting to TMC..."
        else
            echo "Restarting Successfully"
        fi
    fi
}
--> Run, "chmod -x "your_program"" to make it excutable.

################################ Task scheduler & Startup ################################

Instruction: "nano /etc/rc.local"
Description : Startup programs
{
    #!/bin/sh -e
    # rc.local

    # List of programs to run at startup
    /root/bridge_off.sh &
    /root/RVD/version &
}

Instruction: "crontab -e"
Description : Task scheduler
{
    SHELL=/bin/sh
    HOME=/root
    PATH=/usr/local/sbin:usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

    5 * * * * /root/bridge_off.sh # Turn off bridge network every 5 minutes 
    * * * * * /root/keep_alive.sh # Check the RVD program every 1 minute 
}
SHELL=/bin/bash
HOME=/home/apo
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

* * * * * /bin/bash /home/apo/restart_script.sh
service cron restart or systemctl restart cron

Instruction "usbipd wsl attach --busid <id>

Instruction: "ntpdate <ip address>
Description: Set Date to IP