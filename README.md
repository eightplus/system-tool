system-tool
===========

System information viewing tool with Qt5, divided into DBUS background and GUI front.

https://blog.csdn.net/kobe24lmlps/article/details/80777413


Build and Install
============

snapcraft init

snapcraft

snapcraft cleanbuild

snapcraft clean

sudo snap install system-tool_1.0.0_amd64.snap --stable --dangerous

(sudo snap install system-tool_1.0.2_amd64.snap --devmode --dangerous)

sudo snap remove system-tool

unsquashfs system-tool_1.0.0_amd64.snap

snap interfaces

sudo snap connect system-tool:daemon-plug system-tool-daemon:daemon-slot

sudo snap disconnect system-tool:daemon-plug system-tool-daemon:daemon-slot


Publish
============

snapcraft login

snapcraft register system-tool

snapcraft push system-tool_1.0.0_amd64.snap

snapcraft logout


Reference
============

https://docs.snapcraft.io/reference/interfaces

https://forum.snapcraft.io/t/the-dbus-interface/2038

https://github.com/snapcore/snapd/wiki/Interfaces#dbus

https://forum.snapcraft.io/t/how-do-i-connect-a-snap-to-dbus/1533

https://github.com/snapcore/snapd/pull/2592
