#!/bin/bash

adduser --disabled-password --gecos "" so
echo "so:so"|chpasswd
usermod -a -G sudo so

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
apt-get install -y build-essential \
                   linux-headers-$(uname -r) \
                   virtualbox-guest-dkms virtualbox-guest-utils virtualbox-guest-x11 \
                   open-vm-tools open-vm-tools-desktop


# Install basic tools for C/C++ development
apt-get install -y gcc-multilib g++-multilib \
                   git \
                   gdb \
                   valgrind \
                   vim \
                   man-db \
                   manpages-posix \
                   manpages-dev \
                   manpages-posix-dev


# Install misc Ubuntu apps
apt-get install -y fonts-ubuntu \
                   #ttf-ubuntu-font-family \
                   curl \
                   wget \
                   net-tools \
                   network-manager \
                   network-manager-gnome \
                   openjdk-11-jdk pdftk-java \
                   imagemagick \
                   python3-tk \
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
                   teams-for-linux

rm /usr/share/applications/display-im6*.desktop

snap install snap-store


# Install basic web development and hacking tools
apt-get install -y php-cli \
                   python3-pip \
                   golang-go \
                   apache2 \
                   ufw \
                   nmap \
                   sqlmap \
                   wireshark

systemctl disable apache2
systemctl stop apache2

snap install ngrok
snap install zaproxy --classic
snap install hetty

pip3 install pwntools




# Install Visual Studio Code and its C/C++ extension
snap install --classic code
su - so -c 'code --install-extension ms-vscode.cpptools'
su - so -c 'mkdir -p /home/so/.config/Code/User && cd /home/so/.config/Code/User && jq -r '"'"'."security.workspace.trust.enabled" |= false'"'"' settings.json > settings.json.tmp && mv settings.json.tmp settings.json'


# Install Docker
#apt-get install -y ca-certificates gnupg lsb-release
#mkdir -p /etc/apt/keyrings
#curl -fsSL https://download.docker.com/linux/ubuntu/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg
#echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null
#apt-get update
#apt-get install docker-ce docker-ce-cli containerd.io docker-compose-plugin

usermod -a -G docker so


# Install PowerShell
apt-get install -y wget apt-transport-https software-properties-common
wget -q "https://packages.microsoft.com/config/ubuntu/$(lsb_release -rs)/packages-microsoft-prod.deb"
dpkg -i packages-microsoft-prod.deb
rm -f packages-microsoft-prod.deb
apt-get update
apt-get install -y powershell


# Install QEMU and virt-manager
apt-get install -y qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virt-manager


# Install password manager for GIT
apt-get install -y libsecret-tools libsecret-common libsecret-1-0 libsecret-1-dev
wget https://github.com/GitCredentialManager/git-credential-manager/releases/download/v2.0.785/gcm-linux_amd64.2.0.785.deb
dpkg -i gcm-linux_amd64.2.0.785.deb
rm -f gcm-linux_*.deb

su - so -c 'git-credential-manager-core configure'
su - so -c 'git config --global credential.credentialStore secretservice'


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


# Install "dbus-launch" (to change the GNOME settings)
env DEBIAN_FRONTEND=noninteractive apt-get install -y dbus-x11


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


# Disable the screensaver
su - so -c "dbus-launch gsettings set org.gnome.desktop.session idle-delay 0"
su - so -c "dbus-launch gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-type 'nothing'"


# Set region and keyboard layout for GNOME
su - so -c "dbus-launch gsettings set org.gnome.system.locale region 'it_IT.utf8'"
su - so -c "dbus-launch gsettings set org.gnome.desktop.input-sources sources \"[('xkb', 'it')]\""


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
netplan apply



apt-get update
apt-get autoremove -y
shutdown -r now

