#!/bin/bash

# Disable configuration wizards in apt (use defaults)
export DEBIAN_FRONTEND=noninteractive

# Create user "so"
adduser --disabled-password --gecos "" so
echo "so:so"|chpasswd
usermod -a -G sudo so

# Install updates
apt-get update
apt-get upgrade -y


# Install Ubuntu Desktop
apt-get install -y --no-install-recommends ubuntu-desktop


# Set automatic login for user "so"
perl -p -i -e 's/#\s*AutomaticLogin/AutomaticLogin/' /etc/gdm3/custom.conf
perl -p -i -e 's/AutomaticLogin\s=\s\w+/AutomaticLogin = so/' /etc/gdm3/custom.conf


# Install Italian language support for Ubuntu, set locale
apt-get install -y language-pack-it language-pack-gnome-it language-pack-it-base language-pack-gnome-it-base
update-locale LANG=it_IT.UTF-8 LANGUAGE= LC_MESSAGES= LC_COLLATE= LC_CTYPE=


# Install guest OS tools for VirtualBox and VMware
apt-get install -y build-essential
apt-get install -y linux-headers-$(uname -r)
apt-get install -y virtualbox-guest-utils virtualbox-guest-x11
#apt-get install -y virtualbox-dkms virtualbox-guest-additions-iso
apt-get install -y open-vm-tools open-vm-tools-desktop


# Guest OS tools for QEMU
#https://www.snel.com/support/nstall-qemu-guest-agent-for-debian-ubuntu/
#apt-get install -y qemu-guest-agent
#systemctl enable qemu-guest-agent
#systemctl start qemu-guest-agent


# Install basic tools for C/C++ development
apt-get install -y gcc-multilib g++-multilib \
                   git \
                   gdb \
                   valgrind \
                   vim \
                   man-db \
                   manpages-posix \
                   manpages-dev \
                   manpages-posix-dev \
                   llvm \
                   lldb \
                   clang \
                   glibc-tools \
                   glibc-source

#cd /usr/src/glibc
#tar xf glibc-*.tar.xz
#GLIBC_SOURCE_DIR=`pwd`/`ls -d */|grep "glibc-"`
#echo "dir ${GLIBC_SOURCE_DIR}" >> /etc/gdb/gdbinit
#cd -


# GDB will be attached to both processes after a call to fork()
su - so -c 'echo "set detach-on-fork off" >> /home/so/.gdbinit'

# Set all threads of all processes to be resumed when an execution command is issued
su - so -c 'echo "set schedule-multiple on" >> /home/so/.gdbinit'


# Install misc Ubuntu apps
apt-get install -y fonts-ubuntu \
                   curl \
                   wget \
                   tree \
                   dos2unix \
                   expect \
                   net-tools \
                   network-manager \
                   network-manager-gnome \
                   jq \
                   firefox \
                   firefox-locale-it \
                   chromium-browser \
                   chromium-browser-l10n \
                   hunspell-it \
                   witalian \
                   evince \
                   file-roller \
                   gedit \
                   gnome-system-monitor \
                   gnome-logs \
                   seahorse \
                   eog \
                   eog-plugins

# was: ttf-ubuntu-font-family

snap install snap-store

snap install teams-for-linux


# Misc packages for examples from lectures
apt-get install -y cgroup-tools \
                   openjdk-11-jdk \
                   pdftk-java \
                   poppler-utils \
                   imagemagick \
                   python3-tk \
                   mathomatic-primes


# Annotate default wallpaper with text
cp /usr/share/backgrounds/warty-final-ubuntu.png /usr/share/backgrounds/warty-final-ubuntu-orig.png
convert -pointsize 96 -fill "#FFFFFF" -gravity SouthWest -annotate +300+100 "VM Corso SO\nusername: so\npassword: so" /usr/share/backgrounds/warty-final-ubuntu-orig.png  /usr/share/backgrounds/warty-final-ubuntu.png


# Install basic web development and hacking tools
apt-get install -y php-cli \
                   python3-pip \
                   golang-go \
                   apache2 \
                   ufw \
                   nmap \
                   sqlmap \
                   wireshark \
                   tshark \
                   python3-venv


systemctl disable apache2
systemctl stop apache2

snap install ngrok
snap install zaproxy --classic
snap install hetty

pip3 install pwntools
pip3 install flask
pip3 install semgrep

apt -y install python3-requests python3-bs4

pip3 install --upgrade requests



# Install Visual Studio Code and its C/C++ extension
snap install --classic code
su - so -c 'code --install-extension ms-vscode.cpptools'

# Default settings for Visual Studio Code (hide ".vscode" folder, disable workspace trust)
#su - so -c 'mkdir -p /home/so/.config/Code/User && cd /home/so/.config/Code/User && touch settings.json && jq -r '"'"'."security.workspace.trust.enabled" |= false'"'"' settings.json > settings.json.tmp && jq -r '"'"'."files.exclude" |= { "**/.vscode": true }'"'"' settings.json.tmp > settings.json.tmp && mv settings.json.tmp settings.json'
su - so -c 'mkdir -p /home/so/.config/Code/User && touch /home/so/.config/Code/User/settings.json'

cat <<EOF >/home/so/.config/Code/User/settings.json
{
    "security.workspace.trust.enabled": false,
    "files.exclude": {
        "**/.vscode": true,
        "**/.git": true,
        "**/.github": true,
        "**/.test": true
    },
    "extensions.ignoreRecommendations": true
}
EOF


# Install Docker
#apt-get install -y ca-certificates gnupg lsb-release
#mkdir -p /etc/apt/keyrings
#curl -fsSL https://download.docker.com/linux/ubuntu/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg
#echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null
#apt-get update
#apt-get install docker-ce docker-ce-cli containerd.io docker-compose-plugin

groupadd docker
usermod -a -G docker so

chown root:root /usr/local/bin/docker-compose*
chmod a+r /usr/local/bin/docker-compose*


# Install PowerShell
apt-get install -y wget apt-transport-https software-properties-common
wget -q "https://packages.microsoft.com/config/ubuntu/$(lsb_release -rs)/packages-microsoft-prod.deb"
dpkg -i packages-microsoft-prod.deb
rm -f packages-microsoft-prod.deb
apt-get update
apt-get install -y powershell


# Install QEMU and virt-manager
apt-get install -y qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virt-manager

# Note: disabling libvirtd on boot, because of high CPU consumption
# https://www.linuxquestions.org/questions/linux-virtualization-and-cloud-90/how-do-i-disable-libvirtd-from-auto-starting-at-boot-and-stay-disabled-4175660392/
systemctl disable libvirtd
systemctl disable libvirt-guests
systemctl stop libvirtd


# Install misc Linux debugging tools
apt-get install -y linux-tools-common
apt-get install -y linux-tools-$(uname -r)


# Install password manager for GIT
apt-get install -y libsecret-tools libsecret-common libsecret-1-0 libsecret-1-dev

# fixes: https://stackoverflow.com/questions/73312785/dotnet-sdk-is-installed-but-not-recognized-linux-ubuntu-popos-22-04
apt -y remove 'dotnet*'
apt -y remove 'aspnetcore*'
rm -f /etc/apt/sources.list.d/microsoft-prod.list
apt update

curl -L https://aka.ms/gcm/linux-install-source.sh -O
bash linux-install-source.sh -y
rm -f linux-install-source.sh

su - so -c 'git-credential-manager configure'
su - so -c 'git config --global credential.credentialStore plaintext'
#su - so -c 'git config --global credential.credentialStore secretservice'
#su - so -c 'git config --global credential.credentialStore cache'


# Install tools and sources for compiling the Linux kernel
apt-get install -y qtcreator qtbase5-dev qt5-qmake cmake
rm /usr/share/applications/*qt*desktop

perl -p -i -e 's/^#\s*deb-src/deb-src/' /etc/apt/sources.list
apt-get update
apt-get -y build-dep linux linux-image-generic
perl -p -i -e 's/^deb-src/# deb-src/' /etc/apt/sources.list
apt-get update

wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.4.210.tar.gz
mv linux-5.4.210.tar.gz /usr/src/


# Disable unused folder in the home (Music, Templates, Videos, etc)
perl -p -i -e 's/^(TEMPLATES=)/#$1/' /etc/xdg/user-dirs.defaults
perl -p -i -e 's/^(PUBLICSHARE=)/#$1/' /etc/xdg/user-dirs.defaults
perl -p -i -e 's/^(MUSIC=)/#$1/' /etc/xdg/user-dirs.defaults
perl -p -i -e 's/^(PICTURES=)/#$1/' /etc/xdg/user-dirs.defaults
perl -p -i -e 's/^(VIDEOS=)/#$1/' /etc/xdg/user-dirs.defaults



# Enable password authentication with SSHd
perl -p -i -e 's/PasswordAuthentication\s\w+/PasswordAuthentication yes/' /etc/ssh/sshd_config
perl -p -i -e 's/ChallengeResponseAuthentication\s\w+/ChallengeResponseAuthentication no/' /etc/ssh/sshd_config



# Install "dbus-launch" (to change the GNOME settings)
apt-get install -y dbus-x11


# Create "shutdown" shortcut on the desktop
mkdir /home/so/Desktop
chown so:so /home/so/Desktop
touch /home/so/Desktop/shutdown.desktop
chown so:so /home/so/Desktop/shutdown.desktop

cat <<EOF >/home/so/Desktop/shutdown.desktop
[Desktop Entry]
Name=Spegni Macchina Virtuale
Exec=gnome-session-quit --power-off
Comment=Spegni Macchina Virtuale
Terminal=false
Type=Application
Icon=system-shutdown
EOF

su - so -c "dbus-launch gio set /home/so/Desktop/shutdown.desktop metadata::trusted true"
chmod a+x /home/so/Desktop/shutdown.desktop


# Modify "favorite apps" on the dock bar (on the left)
su - so -c "dbus-launch gsettings set org.gnome.shell favorite-apps \"['org.gnome.Nautilus.desktop', 'org.gnome.Terminal.desktop', 'code_code.desktop']\""

rm /usr/share/applications/display-im6*.desktop


# Disable the screensaver
su - so -c "dbus-launch gsettings set org.gnome.desktop.session idle-delay 0"
su - so -c "dbus-launch gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'nothing'"


# Set region and keyboard layout for GNOME
su - so -c "dbus-launch gsettings set org.gnome.system.locale region 'it_IT.utf8'"
su - so -c "dbus-launch gsettings set org.gnome.desktop.input-sources sources \"[('xkb', 'it')]\""


# Remove unneeded packages
apt-get update
apt-get autoremove -y


# Set DHCP for all virtual Ethernet NICs
#cat <<EOF >/etc/systemd/network/99-wildcard.network
#[Match]
#Name=en*
#
#[Network]
#DHCP=yes
#EOF

# Set DHCP for all virtual Ethernet NICs (Netplan)
# https://askubuntu.com/questions/1312096/wired-network-settings-missing-in-ubuntu-desktop-20-10
# https://serverfault.com/questions/923328/is-there-a-way-to-automatically-add-network-interfaces-to-systemd-networkd-and-o
# https://askubuntu.com/questions/1373687/automatic-network-card-configuration
# https://askubuntu.com/questions/71159/network-manager-says-device-not-managed
# https://askubuntu.com/questions/1290471/ubuntu-ethernet-became-unmanaged-after-update
rm -f /etc/netplan/*.yaml
cat <<EOF >/etc/netplan/01-wildcard.yaml
network:
    version: 2
    renderer: NetworkManager
    ethernets:
        zz-all-en-1:
            match:
                name: "en*"
            dhcp4: true
        zz-all-en-2:
            match:
                name: "en*"
            dhcp4: true
EOF

perl -p -i -e 's/managed=false/managed=true/' /etc/NetworkManager/NetworkManager.conf

echo > /etc/NetworkManager/conf.d/10-globally-managed-devices.conf

netplan generate
netplan apply


# Widget for showing IP address
add-apt-repository -y ppa:nico-marcq/indicator-ip
apt-get update
apt-get -y install python3-indicator-ip gir1.2-appindicator3-0.1


# GRUB vanilla defaults
rm /etc/default/grub.d/50-cloudimg-settings.cfg


# Disable automatic updates for Ubuntu (to prevent that they break the labs)
# https://askubuntu.com/questions/1322292/how-do-i-turn-off-automatic-updates-completely-and-for-real
cat <<EOF >/etc/apt/apt.conf.d/20auto-upgrades
APT::Periodic::Update-Package-Lists "0";
APT::Periodic::Download-Upgradeable-Packages "0";
APT::Periodic::AutocleanInterval "0";
APT::Periodic::Unattended-Upgrade "0";
EOF

shutdown -r now

