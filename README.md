system-tool
===========

System information viewing tool with Qt5, divided into DBUS background and GUI front.


Build and Install
============

snapcraft init
snapcraft
snapcraft cleanbuild
snapcraft clean
sudo snap install system-tool_1.0.0_amd64.snap --stable --dangerous
sudo snap remove system-tool
unsquashfs system-tool_1.0.0_amd64.snap


Publish
============

snapcraft login
snapcraft register system-tool
snapcraft push system-tool_1.0.0_amd64.snap
snapcraft logout
