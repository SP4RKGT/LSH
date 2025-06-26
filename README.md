# LSH
# LSH is an extremely light shell written completely in c++ for embedded systems

Installation(option 1):
Clone this repo with *git clone https://github.com/SP4RKGT/LSH*
run *clang++ -Os -o main main.cpp*
You will now have a file called "main"

Installation(option 2):
Go to https://lshdownload.duckdns.org and click the download button

Optional:
If you want you can move it to /usr/bin/lsh with *install -Dm755 main /usr/bin/lsh*
and make a .desktop file in /usr/share/applications/
On windows: You can move it to any directory(preferably C:\Program Files\)
then add it to %appdata%\Microsoft\Windows\Start Menu\Programs (only for the current user)
or %programdata%\Microsoft\Windows\Start Menu\Programs (for everyone)

Troubleshooting:
You need to have clang and git to compile from scratch, so go and install those.
to use the "install" command you need either root or sudo.
